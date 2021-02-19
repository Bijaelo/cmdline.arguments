
#ifndef NORET
#define NORET
#endif

template <typename... Args>
inline void NORET stopv(const char*, int, int, Args&&...);

template <typename... Args>
inline void NORET warningw(const char*, int, int, Args&&...);
