# fast_any
A faster alternative to boost:any / std::any

# Benchmarks

From my benchmarks in <a href=https://github.com/cross-platform/dspatch>DSPatch</a>, `fast_any` is over 25% faster than `std::any`

`std::any`:
```
0x Buffer, 10000 Components: 0.192569ms
1x Buffer, 10000 Components: 0.200076ms
2x Buffer, 10000 Components: 0.109053ms
3x Buffer, 10000 Components: 0.078341ms
4x Buffer, 10000 Components: 0.061341ms
```

`fast_any`:
```
0x Buffer, 10000 Components: 0.121448ms
1x Buffer, 10000 Components: 0.131581ms
2x Buffer, 10000 Components: 0.077578ms
3x Buffer, 10000 Components: 0.058087ms
4x Buffer, 10000 Components: 0.045322ms
```

- Test code: https://github.com/cross-platform/dspatch/blob/master/tests/main.cpp
- Test branch using `std::any`: https://github.com/cross-platform/dspatch/pull/53
