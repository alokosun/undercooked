//Author: Claude
//handles translation between protocol structs and bytes in a buffer

// serialization.h - explicit, bounds-checked, network-byte-order (big-endian) I/O.
//
// Never cast structs to bytes: padding and endianness differ between the Windows
// laptops (x86) and the Pi (ARM). Write and read field by field instead.
//
// Both classes use a sticky failure flag instead of exceptions: after any overflow /
// underflow ok() is false, further operations are no-ops (reads return 0), and the
// caller checks ok() once after parsing a whole message.

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

class Buffer_Writer {
  public:
    Buffer_Writer(uint8_t* buf, size_t capacity) : buf_(buf), cap_(capacity) {}

    bool ok() const { return ok_; }
    size_t size() const { return pos_; }
    size_t remaining() const { return cap_ - pos_; }
    const uint8_t* data() const { return buf_; }
    void fail() { ok_ = false; }

    void u8(uint8_t v) { put(&v, 1); }
    void u16(uint16_t v) {
      const uint8_t b[2] = {uint8_t(v >> 8), uint8_t(v)};
      put(b, 2);
    }
    void u32(uint32_t v) {
      const uint8_t b[4] = {uint8_t(v >> 24), uint8_t(v >> 16), uint8_t(v >> 8), uint8_t(v)};
      put(b, 4);
    }
    void u64(uint64_t v) {
      u32(uint32_t(v >> 32));
      u32(uint32_t(v));
    }
    void i8(int8_t v) { u8(uint8_t(v)); }
    void i16(int16_t v) { u16(uint16_t(v)); }
    void i32(int32_t v) { u32(uint32_t(v)); }
    void boolean(bool v) { u8(v ? 1 : 0); }
    void f32(float v) {
      static_assert(sizeof(float) == 4, "float must be 32-bit");
      uint32_t bits;
      std::memcpy(&bits, &v, 4);
      u32(bits);
    }
    // u8 length prefix + raw characters. Fails if longer than max_len (or 255).
    void str(const std::string& s, size_t max_len) {
      if (s.size() > max_len || s.size() > 255) { ok_ = false; return; }
      u8(uint8_t(s.size()));
      put(s.data(), s.size());
    }
    void bytes(const void* p, size_t n) { put(p, n); }

  private:
    void put(const void* p, size_t n) {
      if (!ok_) return;
      if (n > cap_ - pos_) { ok_ = false; return; }
      if (n) std::memcpy(buf_ + pos_, p, n);
      pos_ += n;
    }

    uint8_t* buf_;
    size_t cap_;
    size_t pos_ = 0;
    bool ok_ = true;
};

class Buffer_Reader {
  public:
    Buffer_Reader(const uint8_t* data, size_t len) : data_(data), len_(len) {}

    bool ok() const { return ok_; }
    size_t remaining() const { return len_ - pos_; }
    bool at_end() const { return pos_ == len_; }
    void fail() { ok_ = false; }

    uint8_t u8() {
      uint8_t v = 0;
      take(&v, 1);
      return v;
    }
    uint16_t u16() {
      uint8_t b[2] = {0, 0};
      if (!take(b, 2)) return 0;
      return uint16_t((b[0] << 8) | b[1]);
    }
    uint32_t u32() {
      uint8_t b[4] = {0, 0, 0, 0};
      if (!take(b, 4)) return 0;
      return (uint32_t(b[0]) << 24) | (uint32_t(b[1]) << 16) | (uint32_t(b[2]) << 8) | uint32_t(b[3]);
    }
    uint64_t u64() {
      uint64_t hi = u32();
      uint64_t lo = u32();
      return (hi << 32) | lo;
    }
    int8_t i8() { return int8_t(u8()); }
    int16_t i16() { return int16_t(u16()); }
    int32_t i32() { return int32_t(u32()); }
    // Strict: only 0 or 1 are valid; anything else fails the reader.
    bool boolean() {
      uint8_t v = u8();
      if (v > 1) ok_ = false;
      return v == 1;
    }
    float f32() {
      uint32_t bits = u32();
      float v;
      std::memcpy(&v, &bits, 4);
      return v;
    }
    bool str(std::string& out, size_t max_len) {
      uint8_t n = u8();
      if (!ok_) return false;
      if (n > max_len || n > remaining()) { ok_ = false; return false; }
      out.assign(reinterpret_cast<const char*>(data_ + pos_), n);
      pos_ += n;
      return true;
    }
    bool bytes(void* out, size_t n) { return take(out, n); }
    bool skip(size_t n) {
      if (!ok_ || n > remaining()) { ok_ = false; return false; }
      pos_ += n;
      return true;
    }

  private:
    bool take(void* out, size_t n) {
      if (!ok_ || n > len_ - pos_) { ok_ = false; return false; }
      if (n) std::memcpy(out, data_ + pos_, n);
      pos_ += n;
      return true;
    }

    const uint8_t* data_;
    size_t len_;
    size_t pos_ = 0;
    bool ok_ = true;
  };

// ---- Fixed-point helpers (keep snapshots small) ----
inline int16_t quantize_i16(float v, float scale) {
  float s = v * scale;
  if (!(s == s)) return 0;  // NaN
  if (s > 32767.0f) s = 32767.0f;
  if (s < -32768.0f) s = -32768.0f;
  return int16_t(s >= 0 ? s + 0.5f : s - 0.5f);
}
inline float dequantize_i16(int16_t q, float scale) { return float(q) / scale; }
