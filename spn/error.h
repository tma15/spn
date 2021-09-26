#pragma once


namespace spn {


#define SPN_RUNTIME_ERROR(errmsg)                                        \
  std::cerr << __FILE__ << "(" << __LINE__ << ")" << errmsg << std::endl;\
  exit(1);


#define SPN_ABORT_IF_NOT_EQ(x, y, errmsg)                                  \
  if (x == y) {                                                            \
  } else {                                                                 \
    std::cerr << __FILE__ << "(" << __LINE__ << ")" << errmsg << std::endl;\
    exit(x);                                                               \
  }                                                                        

  
} // namespace spn
