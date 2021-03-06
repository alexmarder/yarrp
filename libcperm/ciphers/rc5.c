/*  ------------------------------------------------------------------------
	libcperm - A library for creating random permutations.
	Copyright (c) 2014, Lance Alt

	This file is part of libcperm.

	This library is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this library.  If not, see <http://www.gnu.org/licenses/>.
	------------------------------------------------------------------------
*/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../cperm.h"
#include "../cperm-internal.h"
#include "rc5-16.h"

struct rc5_data {
	struct rc5_key key;
	uint32_t* blocks;
	uint32_t index;
};

int perm_rc5_create(struct cperm_t* pt) {
	struct rc5_data* d;

	if(pt->key_len != RC5_KEYLEN) {
		return PERM_ERROR_BAD_KEY_LENGTH;
	}

	d = malloc(sizeof(*d));
	memcpy(&d->key, pt->key, pt->key_len);

	pt->cipher_data = d;
	rc5_setup(pt->cipher_data);

	return 0;
}

int perm_rc5_destroy(struct cperm_t* pt) {
	free(pt->cipher_data);
	return 0;
}

int perm_rc5_enc(struct cperm_t* perm, uint32_t pt, uint32_t* ct) {
	struct rc5_data* d = perm->cipher_data;
	rc5_ecb_encrypt(&d->key, &pt, ct);
	return 0;
}

int perm_rc5_dec(struct cperm_t* perm, uint32_t ct, uint32_t* pt) {
	struct rc5_data* d = perm->cipher_data;
	rc5_ecb_decrypt(&d->key, &ct, pt);
	return 0;
}

