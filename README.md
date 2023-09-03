# fast_any
A faster alternative to boost:any / std::any

# Benchmarks

From my benchmarks in <a href=https://github.com/cross-platform/dspatch>DSPatch</a>, `fast_any` is over 25% faster than `std::any`

`std::any`:
```
0x Buffer, 10000 Components: 0.385436ms
1x Buffer, 10000 Components: 0.394041ms
2x Buffer, 10000 Components: 0.23258ms
3x Buffer, 10000 Components: 0.185374ms
4x Buffer, 10000 Components: 0.146045ms
```

`fast_any`:
```
0x Buffer, 10000 Components: 0.278271ms
1x Buffer, 10000 Components: 0.290789ms
2x Buffer, 10000 Components: 0.16599ms
3x Buffer, 10000 Components: 0.138325ms
4x Buffer, 10000 Components: 0.109003ms
```

- Test code: https://github.com/cross-platform/dspatch/blob/master/tests/main.cpp#L1230
- Test branch using `std::any`: https://github.com/cross-platform/dspatch/pull/53
