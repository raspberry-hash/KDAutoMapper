# Kernel Driver Auto Mapper

hi you button-clicking baboon


## Usage

```
SkiddedExternal/
├── source/
│   └── driver/
│       └── kdMapperFiles/
│           ├── kdmapper.exe ← Compile this on ur own 
│           └── RickOwens00.sys 
└── main.cpp
```
```cpp
if (runKdmapper()) {
    printSuccess("Driver mapped successfully.");
} else {
    printError("Failed to map the driver.");
    sleep_ms(5000);
    exit(0);
}
```

