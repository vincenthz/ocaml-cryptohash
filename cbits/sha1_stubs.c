/*
 *	Copyright (C) 2006-2009 Vincent Hanquez <tab@snarc.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; version 2.1 or version 3.0 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Sha1 implementation
 */

#include <unistd.h>
#include <fcntl.h>
#include "sha1.h"

static inline int sha1_file(char *filename, uint8_t *digest)
{
	#define BLKSIZE 4096
	unsigned char buf[BLKSIZE];
	int fd; ssize_t n;
	struct sha1_ctx ctx;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return 1;
	sha1_init(&ctx);
	while ((n = read(fd, buf, BLKSIZE)) > 0)
		sha1_update(&ctx, buf, n);
	if (n == 0)
		sha1_finalize(&ctx, digest);
	close(fd);
	return n < 0;
	#undef BLKSIZE
}

/* this part implement the OCaml binding */
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>

#define GET_CTX_STRUCT(a) ((struct sha1_ctx *) a)

CAMLexport value stub_sha1_init(value unit)
{
	CAMLparam1(unit);
	CAMLlocal1(result);

	result = caml_alloc(sizeof(struct sha1_ctx), Abstract_tag);
	sha1_init(GET_CTX_STRUCT(result));

	CAMLreturn(result);
}

CAMLprim value stub_sha1_update(value ctx, value data, value ofs, value len)
{
	CAMLparam4(ctx, data, ofs, len);

	sha1_update(GET_CTX_STRUCT(ctx), String_val(data) + Int_val(ofs), Int_val(len));
	CAMLreturn(Val_unit);
}

CAMLprim value stub_sha1_finalize(value ctx)
{
	CAMLparam1(ctx);
	CAMLlocal1(result);

	result = caml_alloc_string(20);
	sha1_finalize(GET_CTX_STRUCT(ctx), String_val(result));

	CAMLreturn(result);
}

CAMLprim value stub_sha1_file(value name)
{
	CAMLparam1(name);
	CAMLlocal1(result);

	result = caml_alloc_string(20);
	if (sha1_file(String_val(name), String_val(result)))
		caml_failwith("file error");

	CAMLreturn(result);
}

CAMLprim value stub_sha1_to_hex(value digest)
{
	CAMLparam1(digest);
	CAMLlocal1(result);
	char *s, r;
	int i;

	result = caml_alloc_string(40);
	s = String_val(digest);
	r = String_val(result);
	for (i = 0; i < 20; i++, r += 2)
		snprintf(r, 2, "%02x", s[i]);
	CAMLreturn(result);
}
