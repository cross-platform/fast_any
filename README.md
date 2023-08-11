# fast_any
A faster alternative to boost:any / std::any

# Benchmarks

From my benchmarks in <a href=https://github.com/cross-platform/dspatch>DSPatch</a>, `fast_any` is roughly 30% faster than `std::any`

`std::any`:
```
0x Buffer, 10000 Components (Series Mode): 0.500689ms
1x Buffer, 10000 Components (Series Mode): 0.51738ms
2x Buffer, 10000 Components (Series Mode): 0.770797ms
3x Buffer, 10000 Components (Series Mode): 0.661798ms
4x Buffer, 10000 Components (Series Mode): 0.521245ms
```

`fast_any`:
```
0x Buffer, 10000 Components (Series Mode): 0.345595ms
1x Buffer, 10000 Components (Series Mode): 0.363131ms
2x Buffer, 10000 Components (Series Mode): 0.598035ms
3x Buffer, 10000 Components (Series Mode): 0.560702ms
4x Buffer, 10000 Components (Series Mode): 0.451474ms
```

(Test code: https://github.com/cross-platform/dspatch/blob/master/tests/main.cpp#L1180)
