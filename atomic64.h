/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

#define LOCK "lock ; "
#include<cstdint>

/*
 * 这里基本和atomic.h一样除了 后缀l变成了后缀q   例如 addl -> addq 这样就从
 * 32位操作，变成了64位操作了。
 * 你需要引入 stdint.h 或者 cstdint
 */

/*
 * Make sure gcc doesn't try to be clever and move things around
 * on us. We need to use _exactly_ the address the user gave us,
 * not some alias that contains the same information.
 */
typedef struct { volatile int64_t counter; } atomic64_t;

/**
 * atomic_add - add integer to atomic variable
 * @param i integer value to add
 * @param v pointer of type atomic_t
 * 
 * Atomically adds i to v.
 */

/*
 * ------------desc-------------
 * addq 64bit addl 32bit
 * ------------------------------
 */
inline void atomic_add64(int64_t i, atomic64_t *v)
{
    __asm__ __volatile__(
        LOCK "addq %1,%0"
        :"=m" (v->counter)
        :"ir" (i), "m" (v->counter));

//  __sync_fetch_and_add(&v->counter,i);
}

/**
 * atomic_sub - subtract the atomic variable
 * @param i integer value to subtract
 * @param v pointer of type atomic_t
 * 
 * Atomically subtracts i from v.
 */
static __inline__ void atomic_sub64(int64_t i, atomic64_t *v)
{
    atomic_add64(-i,v);

    //__asm__ __volatile__(
    //    LOCK "subq %1,%0"
    //    :"=m" (v->counter)
    //    :"ir" (i), "m" (v->counter));
}


/**
 * atomic_add_return - add and return
 * @param v pointer of type atomic_t
 * @param i integer value to add
 *
 * Atomically adds i to v and returns i + v
 *
 *--------------------------------
 * xaddq src dest
 * tmp <- dest
 * src+dest -> dest
 * tmp -> src
 */
 __inline__ int64_t atomic_add_return64(int64_t i, atomic64_t *v)
{
    int64_t  __i = i ;

    __asm__ __volatile__(
            LOCK "xaddq %0,%1"
            :"+r" (i),"=m"(v->counter)
            :"m" (v->counter)
            :"memory"
            );

    //i became the old v->counter
    return __i + i;
}

__inline__ int atomic_sub_return64(int64_t i, atomic64_t *v)
{
    // it shows that normal build didn't inline it
    return atomic_add_return64(-i, v);
    //return __sync_fetch_and_add(&v->counter,-i);
}


/**
 * atomic_sub_and_test - subtract value from variable and test result
 * @param i integer value to subtract
 * @param v pointer of type atomic_t
 *
 * Atomically subtracts i from v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
__inline__ int atomic_sub_and_test64(int64_t i, atomic64_t *v)
{
    unsigned char  c ;
    __asm__ __volatile__(
            LOCK "subq %2,%0;sete %1"
            :"=m"(v->counter),"=qm"(c)
            :"ir" (i), "m" (v->counter)
            :"memory"
            );

    return  c;
}

/**
 * atomic_inc - increment atomic variable
 * @param v pointer of type atomic_t
 *
 * Atomically increments v by 1.
 */
static __inline__ void atomic_inc64(atomic64_t *v)
{
    __asm__ __volatile__(
        LOCK "incq %0"
        :"=m" (v->counter)
        :"m" (v->counter));
}

/**
 * atomic_dec - decrement atomic variable
 * @param v pointer of type atomic_t
 *
 * Atomically decrements v by 1.
 */
static __inline__ void atomic_dec64(atomic64_t *v)
{
    __asm__ __volatile__(
        LOCK "decq %0"
        :"=m" (v->counter)
        :"m" (v->counter));
}

/**
 * atomic_dec_and_test - decrement and test
 * @param v pointer of type atomic_t
 *
 * Atomically decrements v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
static __inline__ int atomic_dec_and_test64(atomic64_t *v)
{
    unsigned char c;

    __asm__ __volatile__(
        LOCK "decq %0; setz %1"
        :"=m" (v->counter), "=qm" (c)
        :"m" (v->counter) : "memory");
    return c;
}

/**
 * atomic_inc_and_test - increment and test
 * @param v pointer of type atomic_t
 *
 * Atomically increments v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
static __inline__ int atomic_inc_and_test64(atomic64_t *v)
{
    unsigned char c;

    __asm__ __volatile__(
        LOCK "incq %0; setz %1"
        :"=m" (v->counter), "=qm" (c)
        :"m" (v->counter) : "memory");
    return c;
}
/**
 * atomic_add_negative - add and test if negative
 * @param v pointer of type atomic_t
 * @param i integer value to add
 *
 * Atomically adds i to v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */
static __inline__ int atomic_add_negative64(int64_t i, atomic64_t *v)
{
    unsigned char c;

    __asm__ __volatile__(
        LOCK "addq %2,%0; sets %1"
        :"=m" (v->counter), "=qm" (c)
        :"ir" (i), "m" (v->counter) : "memory");

    return c;
}

