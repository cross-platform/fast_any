# fast_any
A faster alternative to boost:any / std::any

# Benchmarks

From my benchmarks in <a href=https://github.com/cross-platform/dspatch>DSPatch</a>, `fast_any` is roughly 25% faster than `std::any`

`std::any`:
```
0x Buffer, 10000 Components (Series Mode): 0.396029ms
1x Buffer, 10000 Components (Series Mode): 0.398009ms
2x Buffer, 10000 Components (Series Mode): 0.272335ms
3x Buffer, 10000 Components (Series Mode): 0.225114ms
4x Buffer, 10000 Components (Series Mode): 0.185802ms
```

`fast_any`:
```
0x Buffer, 10000 Components (Series Mode): 0.288617ms
1x Buffer, 10000 Components (Series Mode): 0.29731ms
2x Buffer, 10000 Components (Series Mode): 0.19904ms
3x Buffer, 10000 Components (Series Mode): 0.189002ms
4x Buffer, 10000 Components (Series Mode): 0.155049ms
```

**NOTE:** The 0x and 1x Buffer tests best illustrate a direct comparison of each class's performance.
The decrease in time delta as buffer count increases is due to how DSPatch parallel processes buffers.

- Test code: https://github.com/cross-platform/dspatch/blob/master/tests/main.cpp#L1230
- Test branch using `std::any`: https://github.com/cross-platform/dspatch/pull/53
