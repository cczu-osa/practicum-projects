## Description

Implementation of advanced encryption standard (AES) with electronic codebook mode (ECB).

## Requirement

### For CPU version

Any C++11 compiler will work.

### For GPU version

NVIDIA GPU + CUDA Toolkits. If you are a windows user, I recommend Visual Studio 2015 Community as your IDE.

## Test

Build solutions, and copy `test.ps1` to output folder. Then open PowerShell and type:

```
.\test.ps1
```

The output should be something looking like this:

```
PS C:\AES-128-ECB\x64\Debug> .\test.ps1
[Prepare] Create a random file to encrypt...
[Prepare] Writing data to filesystem...
[TEST][CPU] Done, SHA256 of result is: E5B3D9FF399F26D39F94A64E685384C0DAE0CC4DCE60AFDACF4C105DEAA7B49A
[TEST][CPU] Using CPU to encrypt 100MB file with 8 threads...
[TEST][CPU] Done, SHA256 of result is: F30016C55F49AC3553901B04B8F0691C7963AB4C65BB2A60CBF670B6CBC6B3CD
[TEST][CPU] Using CPU to decrypt 100MB file with 8 threads...
[TEST][CPU] Done, SHA256 of result is: E5B3D9FF399F26D39F94A64E685384C0DAE0CC4DCE60AFDACF4C105DEAA7B49A
[TEST][GPU] Using GPU to encrypt 100MB file...
[TEST][GPU] Done, SHA256 of result is: F30016C55F49AC3553901B04B8F0691C7963AB4C65BB2A60CBF670B6CBC6B3CD
[TEST][GPU] Using GPU to decrypt 100MB file...
[TEST][GPU] Done, SHA256 of result is: E5B3D9FF399F26D39F94A64E685384C0DAE0CC4DCE60AFDACF4C105DEAA7B49A
PS C:\AES-128-ECB\x64\Debug>
```