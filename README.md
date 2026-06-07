# Human-Like Password Variant Generator (C++)

A high-performance C++17 password variant generator designed to create large wordlists from base keywords while maintaining low memory usage through streaming output.

## Features

* Stream-based processing (minimal RAM usage)
* Generates thousands of realistic variants per input word
* Supports common capitalization patterns:

  * lowercase
  * Capitalized
  * UPPERCASE
* Optional leetspeak substitutions:

  * a → 4, @
  * o → 0
  * i → 1, !
  * e → 3
  * s → 5, $
  * t → 7
* Prefix generation
* Suffix generation
* Numeric combinations:

  * 0–9
  * 00–99
  * 000–999
* Common separators:

  * _
  * .
  * *
* Common keywords:

  * admin
  * password
  * root
  * guest
  * welcome
  * qwerty
  * and many more
* Month combinations:

  * jan–dec
* Year combinations:

  * 1970–2026
* Duplicate filtering
* Configurable maximum variants per base word
* Progress reporting during execution
* Designed for very large input files

## Compilation

```bash
g++ -std=c++17 -O3 -march=native generate.cpp -o generate
```

## Usage

```bash
./generate -i input.txt -o output.txt
```

### Advanced Options

```bash
./generate \
  -i input.txt \
  -o output.txt \
  --max-per-word 50000 \
  --flush-every 10000
```

Disable leetspeak generation:

```bash
./generate -i input.txt -o output.txt --no-leet
```

## Example

Input:

```text
john
```

Possible output:

```text
john1
john123
john_007
john2025
John777
JOHN99
j0hn
admin_john
john-password
superjohn
john.sep
janjohn
johndec
```

## Performance

The generator writes results directly to disk and does not store generated variants in memory, allowing it to process very large datasets while maintaining low RAM consumption.

Output size depends on input size and configuration. Large input files may generate tens or even hundreds of gigabytes of output data.

## Disclaimer

This project is intended for educational purposes, password auditing, security research, and recovery of passwords for systems you own or are explicitly authorized to test. Users are responsible for complying with all applicable laws and regulations.
