# Elf executables packing

![](object_file_format.png)

> Apart from the ELF header that is at the beginning of the file, the different elements of an elf can be anywhere as long as they are properly referenced. Section indexes are used throughout the file, for this reason, it is simpler to keep sections (in the section table) in the same order when packing.

# Strategy

Inserting the payload in the memory padding at the end of the last section in the entry's `PT_LOAD` segment.

| File Offset | Distance from Payload      |
| ----------- | -------------------------- |
| `p_offset`  | `relative_pt_load_address` |
| `sh_offset` | `relative_text_address`    |
|             | `relative_entry_address`   |

```
            Packed Elf64 Binary Memory Overview
            ===================================

                          .        .
                          |  ...   |
              p_offset -> |========| PT_LOAD  - relative_pt_load_address
                          |  ...   |          ^
                          |  ...   |          |
             sh_offset -> |--------| .Text    |- relative_text_address
                      |   |  ...   |          |^
                      V   |  ...   |          ||
    offset_in_section -   |>>>>>>>>| entry    ||- relative_entry_address
                          |  ...   |          ||^
                          |--------|          |||
                          |  ...   |          |||
                          |  ...   |          |||
                          |  ...   |          |||
   end_of_last_section -> |@@@@@@@@| payload  |||
                          |@      @| |
                          |@      @| V
                          |@@@@@@@@| payload_size
                          |========|
                          |  ...   |
                          .        .

```
