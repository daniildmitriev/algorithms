# algorithms
4.1
для volatile не нужно синхронизировать кэш => можно использовать relaxed.
для обычных нужно синхронизировать кэш, на чтении достаточно acquire, на записи - release
4.2
2,6 GHz Intel Core i5
4 x 32 KB L1 cache
64 bytes line size
