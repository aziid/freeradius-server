/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef _RLM_EAP_H
#define _RLM_EAP_H
/**
 * $Id$
 * @file rlm_eap.h
 * @brief Implements the EAP framework.
 *
 * @copyright 2000-2003,2006  The FreeRADIUS server project
 * @copyright 2001  hereUare Communications, Inc. <raghud@hereuare.com>
 * @copyright 2003  Alan DeKok <aland@freeradius.org>
 */
RCSIDH(rlm_eap_h, "$Id$")

#include <freeradius-devel/modpriv.h>
#include <freeradius-devel/state.h>
#include "eap.h"
#include "eap_types.h"

/** Private structure to hold handles and interfaces for an EAP method
 *
 */
typedef struct rlm_eap_method {
	CONF_SECTION			*cs;

	dl_module_t const		*submodule_handle;		//!< Submodule's dl_handle.
	void				*submodule_inst;		//!< Submodule's instance data
	rlm_eap_submodule_t const	*submodule;			//!< Submodule's exported interface.
} rlm_eap_method_t;

/** Instance data for rlm_eap
 *
 */
typedef struct rlm_eap {
	rlm_eap_config_t		config;				//!< Configuration for this instance of
									//!< rlm_eap. Must be first in this struct.

	char const			*name;				//!< Name of this instance.

	rlm_eap_method_t 		*methods[PW_EAP_MAX_TYPES];	//!< Array of loaded (or not), submodules.
	fr_randctx			rand_pool;			//!< Pool of random data.
} rlm_eap_t;

/*
 *	EAP Method selection
 */
int      	eap_method_instantiate(rlm_eap_method_t **out, rlm_eap_t *inst, eap_type_t num, CONF_SECTION *cs);

/*
 *	EAP Method composition
 */
int  		eap_start(rlm_eap_t const *inst, REQUEST *request) CC_HINT(nonnull);
void 		eap_fail(eap_session_t *eap_session) CC_HINT(nonnull);
void 		eap_success(eap_session_t *eap_session) CC_HINT(nonnull);
rlm_rcode_t 	eap_compose(eap_session_t *eap_session) CC_HINT(nonnull);

/*
 *	Session management
 */
void		eap_session_destroy(eap_session_t **eap_session);
void		eap_session_freeze(eap_session_t **eap_session);
eap_session_t	*eap_session_thaw(REQUEST *request);
eap_session_t 	*eap_session_continue(eap_packet_raw_t **eap_packet, rlm_eap_t const *inst,
				      REQUEST *request) CC_HINT(nonnull);

/*
 *	Memory management
 */
eap_round_t	*eap_round_alloc(eap_session_t *eap_session) CC_HINT(nonnull);
eap_session_t	*eap_session_alloc(rlm_eap_t const *inst, REQUEST *request) CC_HINT(nonnull);

#endif /*_RLM_EAP_H*/
