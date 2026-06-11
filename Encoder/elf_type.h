#pragma once

#include <stdint.h>

#pragma pack(push, 1)

#define ELFCLASSNONE (0)
#define ELFCLASS32 (1)
#define ELFCLASS64 (2)

#define ELFDATANONE (0)
#define ELFDATA2LSB (1)
#define ELFDATA2MSB (2)

#define ET_NONE (0)
#define ET_REL (1)
#define ET_EXEC (2)
#define ET_DYN (3)

#define E_PHNUM (65535)

/* Type for a 16-bit quantity.  */
typedef uint16_t Elf32_Half;
typedef uint16_t Elf64_Half;

/* Types for signed and unsigned 32-bit quantities.  */
typedef uint32_t Elf32_Word;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf64_Word;
typedef int32_t  Elf64_Sword;

/* Types for signed and unsigned 64-bit quantities.  */
typedef uint64_t Elf32_Xword;
typedef int64_t  Elf32_Sxword;
typedef uint64_t Elf64_Xword;
typedef int64_t  Elf64_Sxword;

/* Type of addresses.  */
typedef uint32_t Elf32_Addr;
typedef uint64_t Elf64_Addr;

/* Type of file offsets.  */
typedef uint32_t Elf32_Off;
typedef uint64_t Elf64_Off;

/* Type for section indices, which are 16-bit quantities.  */
typedef uint16_t Elf32_Section;
typedef uint16_t Elf64_Section;

/* Type for version symbol information.  */
typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;

#define EI_NIDENT (16)

/* ELF Header */
typedef struct elfhdr {
    unsigned char    e_ident[EI_NIDENT]; /* ELF Identification */
    Elf32_Half    e_type;        /* object file type */
    Elf32_Half    e_machine;    /* machine */
    Elf32_Word    e_version;    /* object file version */
    Elf32_Addr    e_entry;    /* virtual entry point */
    Elf32_Off    e_phoff;    /* program header table offset */
    Elf32_Off    e_shoff;    /* section header table offset */
    Elf32_Word    e_flags;    /* processor-specific flags */
    Elf32_Half    e_ehsize;    /* ELF header size */
    Elf32_Half    e_phentsize;    /* program header entry size */
    Elf32_Half    e_phnum;    /* number of program header entries */
    Elf32_Half    e_shentsize;    /* section header entry size */
    Elf32_Half    e_shnum;    /* number of section header entries */
    Elf32_Half    e_shstrndx;    /* section header table's "section
                     header string table" entry offset */
} Elf32_Ehdr;

/* Program Header */
typedef struct {
    Elf32_Word    p_type;        /* segment type */
    Elf32_Off    p_offset;    /* segment offset */
    Elf32_Addr    p_vaddr;    /* virtual address of segment */
    Elf32_Addr    p_paddr;    /* physical address - ignored? */
    Elf32_Word    p_filesz;    /* number of bytes in file for seg. */
    Elf32_Word    p_memsz;    /* number of bytes in mem. for seg. */
    Elf32_Word    p_flags;    /* flags */
    Elf32_Word    p_align;    /* memory alignment */
} Elf32_Phdr;

/* Section Header */
typedef struct {
    Elf32_Word    sh_name;    /* name - index into section header
                       string table section */
    Elf32_Word    sh_type;    /* type */
    Elf32_Word    sh_flags;    /* flags */
    Elf32_Addr    sh_addr;    /* address */
    Elf32_Off    sh_offset;    /* file offset */
    Elf32_Word    sh_size;    /* section size */
    Elf32_Word    sh_link;    /* section header table index link */
    Elf32_Word    sh_info;    /* extra information */
    Elf32_Word    sh_addralign;    /* address alignment */
    Elf32_Word    sh_entsize;    /* section entry size */
} Elf32_Shdr;

/* Symbol Table Entry */
typedef struct elf32_sym {
    Elf32_Word    st_name;    /* name - index into string table */
    Elf32_Addr    st_value;    /* symbol value */
    Elf32_Word    st_size;    /* symbol size */
    unsigned char    st_info;    /* type and binding */
    unsigned char    st_other;    /* 0 - no defined meaning */
    Elf32_Half    st_shndx;    /* section header index */
} Elf32_Sym;



#pragma pack(pop)
