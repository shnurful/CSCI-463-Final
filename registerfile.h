#pragma once
#include <cstdint>
#include <vector>
#include <string>

using std::vector;
using std::string;

class registerfile {
public:
  registerfile();
  void reset();
  void set(uint32_t r, int32_t val);
  int32_t get(uint32_t r) const;
  void dump(const string &hdr="") const;

private:
  vector<int32_t> regs = vector<int32_t>(33,0xf0f0f0f0);
  
};


