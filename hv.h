/*    hv.h
 *
 *    Copyright (c) 1991-1994, Larry Wall
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 */

typedef struct he HE;
typedef struct he_key HEK;

struct he {
    HE		*hent_next;
    HEK		*hent_hk;
    SV		*hent_val;
};

struct he_key {
    U32		hk_hash;
    I32		hk_len;
    char	hk_key[1];
};

struct xpvhv {
    char *	xhv_array;	/* pointer to malloced string */
    STRLEN	xhv_fill;	/* how full xhv_array currently is */
    STRLEN	xhv_max;	/* subscript of last element of xhv_array */
    I32		xhv_keys;	/* how many elements in the array */
    double	xnv_nv;		/* numeric value, if any */
    MAGIC*	xmg_magic;	/* magic for scalar array */
    HV*		xmg_stash;	/* class package */

    I32		xhv_riter;	/* current root of iterator */
    HE		*xhv_eiter;	/* current entry of iterator */
    PMOP	*xhv_pmroot;	/* list of pm's for this package */
    char	*xhv_name;	/* name, if a symbol table */
};

#define PERL_HASH(hash,str,len) \
     STMT_START	{ \
	register char *s_PeRlHaSh = str; \
	register I32 i_PeRlHaSh = len; \
	register U32 hash_PeRlHaSh = 0; \
	while (i_PeRlHaSh--) \
	    hash_PeRlHaSh = hash_PeRlHaSh * 33 + *s_PeRlHaSh++; \
	(hash) = hash_PeRlHaSh; \
    } STMT_END


/* these hash entry flags ride on hent_klen (for use only in magic/tied HVs) */
#define HEf_SVKEY	-2	/* hent_key is a SV* */


#define Nullhv Null(HV*)
#define HvARRAY(hv)	((HE**)((XPVHV*)  SvANY(hv))->xhv_array)
#define HvFILL(hv)	((XPVHV*)  SvANY(hv))->xhv_fill
#define HvMAX(hv)	((XPVHV*)  SvANY(hv))->xhv_max
#define HvKEYS(hv)	((XPVHV*)  SvANY(hv))->xhv_keys
#define HvRITER(hv)	((XPVHV*)  SvANY(hv))->xhv_riter
#define HvEITER(hv)	((XPVHV*)  SvANY(hv))->xhv_eiter
#define HvPMROOT(hv)	((XPVHV*)  SvANY(hv))->xhv_pmroot
#define HvNAME(hv)	((XPVHV*)  SvANY(hv))->xhv_name

#define HvSHAREKEYS(hv)		(SvFLAGS(hv) & SVphv_SHAREKEYS)
#define HvSHAREKEYS_on(hv)	(SvFLAGS(hv) |= SVphv_SHAREKEYS)
#define HvSHAREKEYS_off(hv)	(SvFLAGS(hv) &= ~SVphv_SHAREKEYS)

#define HvLAZYDEL(hv)		(SvFLAGS(hv) & SVphv_LAZYDEL)
#define HvLAZYDEL_on(hv)	(SvFLAGS(hv) |= SVphv_LAZYDEL)
#define HvLAZYDEL_off(hv)	(SvFLAGS(hv) &= ~SVphv_LAZYDEL)

#ifdef OVERLOAD

/* Maybe amagical: */
/* #define HV_AMAGICmb(hv)      (SvFLAGS(hv) & (SVpgv_badAM | SVpgv_AM)) */

#define HV_AMAGIC(hv)        (SvFLAGS(hv) &   SVpgv_AM)
#define HV_AMAGIC_on(hv)     (SvFLAGS(hv) |=  SVpgv_AM)
#define HV_AMAGIC_off(hv)    (SvFLAGS(hv) &= ~SVpgv_AM)

/*
#define HV_AMAGICbad(hv)     (SvFLAGS(hv) & SVpgv_badAM)
#define HV_badAMAGIC_on(hv)  (SvFLAGS(hv) |= SVpgv_badAM)
#define HV_badAMAGIC_off(hv) (SvFLAGS(hv) &= ~SVpgv_badAM)
*/

#endif /* OVERLOAD */

#define Nullhe Null(HE*)
#define HeNEXT(he)		(he)->hent_next
#define HeKEY_hk(he)		(he)->hent_hk
#define HeKEY(he)		HK_KEY(HeKEY_hk(he))
#define HeKEY_sv(he)		(*(SV**)HeKEY(he))
#define HeKLEN(he)		HK_LEN(HeKEY_hk(he))
#define HeVAL(he)		(he)->hent_val
#define HeHASH(he)		HK_HASH(HeKEY_hk(he))
#define HePV(he)		((HeKLEN(he) == HEf_SVKEY) ?		\
				 SvPV(HeKEY_sv(he),na) :		\
				 HeKEY(he))
#define HeSVKEY(he)		((HeKEY(he) && 				\
				  HeKLEN(he) == HEf_SVKEY) ?		\
				 HeKEY_sv(he) : Nullsv)

#define HeSVKEY_force(he)	(HeKEY(he) ?				\
				 ((HeKLEN(he) == HEf_SVKEY) ?		\
				  HeKEY_sv(he) :			\
				  sv_2mortal(newSVpv(HeKEY(he),		\
						     HeKLEN(he)))) :	\
				 &sv_undef)
#define HeSVKEY_set(he,sv)	(HeKEY_sv(he) = sv)

#define HK_LEN(hk)		(hk)->hk_len
#define HK_KEY(hk)		(hk)->hk_key
#define HK_HASH(hk)		(hk)->hk_hash
