#include <asm.h>
#include <segment.h>
#include <types.h>



#define wrmsr(msr,val1,val2) \
     _asm_ _volatile_("wrmsr" \
			  : /* no outputs */ \
			  : "c" (msr), "a" (val1), "d" (val2))

static inline void wrmsrl(unsigned int msr, int val)
{
        __u32 lo, hi;
        lo = (__u32)val;
        hi = (__u32)(val >> 32);
        wrmsr(msr, lo, hi);
}
