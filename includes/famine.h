#ifndef FAMINE_H
# define FAMINE_H

# include "tools.h"

# include <elf.h>

# include <sys/mman.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>
# include <sys/syscall.h>

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# define BYTECODE	"" // to remove

# define KEY_SIZE 64

# define VIRUS_NAME "famine"

# define NEW_BIN_FILENAME "woody"
# define BYTECODE_SIZE sizeof(BYTECODE)

# define PATTERN_ENTRY_OLD		0xAAAAAAe9
# define PATTERN_ENTRY_OLD_SIZE_OPCODE	1

# define PATTERN_KEY_SIZE		0xBBBBBBBBBBBBBBBB
# define PATTERN_KEY_SIZE_OPCODE	0

# define PATTERN_TEXT_SIZE		0xCCCCCCCCCCCCCCCC
# define PATTERN_TEXT_SIZE_OPCODE	0

# define PATTERN_ENTRY_TEXT		0xDDDDDDDD
# define PATTERN_ENTRY_TEXT_SIZE_OPCODE	0

typedef enum {false, true} bool;

typedef struct	s_key {
	char	raw[KEY_SIZE];
	size_t	length;
}		t_key;

typedef struct	s_woody {
	char		*target;
	struct s_key	key;
	struct stat	bin_st;
	void		*bin_map;
	Elf64_Ehdr	ehdr;		// Ehdr copy with good endian
	Elf64_Shdr	shstrtab;	// Shdr copy with good endian
	Elf64_Shdr	new_section;
	uint64_t	new_entry;
	uint64_t	shdr_last_offset_adjustment;
	uint64_t	new_section_and_padding_size;
	bool		reverse_endian;
}		t_woody;

typedef struct	s_famine {
	t_woody		woody;
}		t_famine;

void	exit_clean(struct s_woody *woody, int exit_status);

void	check_headers_offset(struct s_woody *woody);

const char	*get_phdr_type_str(uint32_t p_type);
const char	*get_phdr_flags_str(uint32_t flags, char buff[4]);
const char	*get_ehdr_type_str(uint16_t e_type);
const char	*get_shdr_type_str(uint32_t sh_type);
void		get_shstrtab(struct s_woody *woody);
uint16_t	get_index_segment_containing_section(struct s_woody *woody, Elf64_Shdr *section);
uint16_t	get_index_section_with_name(struct s_woody *woody, char *section_name);

void	read_elf_header(struct s_woody *woody);
void	read_program_header(struct s_woody *woody, uint16_t index, Elf64_Phdr *phdr);
void	read_section_header(struct s_woody *woody, uint16_t index, Elf64_Shdr *shdr);

void	write_uint16(struct s_woody *woody, uint16_t *addr, uint16_t value);
void	write_uint32(struct s_woody *woody, uint32_t *addr, uint32_t value);
void	write_uint64(struct s_woody *woody, uint64_t *addr, uint64_t value);

void	modify_ehdr(struct s_woody *woody);
void	modify_phdr_bss(struct s_woody *woody, Elf64_Phdr *phdr_bss, uint16_t index_phdr_bss);
void	modify_phdr_text(struct s_woody *woody, Elf64_Shdr *shdr_text);
void	modify_shdr_pushed_by_new_section(struct s_woody *woody, uint16_t index_shdr_last);
void	modify_shdr_last(struct s_woody *woody, Elf64_Shdr *shdr_last, uint16_t index_shdr_last);

void	insert_section_after_bss(struct s_woody *woody);

void	save_new_section(struct s_woody *woody, int new_bin_fd, Elf64_Shdr *shdr_bss);
void	save_new_shdr(struct s_woody *woody, int new_bin_fd, Elf64_Shdr *new_section);
void	save_new_elf_file(struct s_woody *woody, Elf64_Shdr *shdr_bss, uint16_t index_shdr_bss);

// DEBUG
void	debug_print_headers(struct s_woody *woody);
void	debug_print_program_header(struct s_woody *woody);
void	debug_print_section_header(struct s_woody *woody);

// asm func
void	xor_cipher(char *key, size_t key_size, void *text, size_t text_size);
long	syscall_wrapper(long number, ...);

#endif