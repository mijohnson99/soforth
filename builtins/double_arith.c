#ifdef EMULATE_DOUBLES
static const ucell_t lohalf = (1UL << 4 * sizeof(cell_t)) - 1;
#define LOAD_DOUBLE(p) ((p)[0] << 4 * sizeof(cell_t) | (p)[1])
#define STORE_DOUBLE(p,v) \
	do { \
		udcell_t a = (v); \
		*(ucell_t *)&(p)[0] = a >> 4 * sizeof(cell_t); \
		*(ucell_t *)&(p)[1] = a & lohalf; \
	} while (0)
#else
#define LOAD_DOUBLE(p) (*(dcell_t *)(p))
#define STORE_DOUBLE(p,v) \
	do { \
		*(udcell_t *)(p) = (udcell_t)(v); \
	} while (0)
#endif

	ASMLABEL(um_divmod_code);
um_divmod_code: /*: UM/MOD ( um_divmod ) ;*/
#if defined(__x86_64__) && !defined(EMULATE_DOUBLES)
	sp--;
	asm ("divq %4"
		:"=d"(sp[-1]), "=a"(tos)
		:"d"(sp[0]), "a"(sp[-1]), "r"(tos));
#else
	udcell_t d = LOAD_DOUBLE(&sp[-2]);
	sp -= 2;
	PUSH(sp) = d % tos;
	tos = d / tos;
#endif
	NEXT();

	ASMLABEL(m_add_code);
m_add_code: /*: M+ ( m_add ) ;*/
	STORE_DOUBLE(&sp[-2], LOAD_DOUBLE(&sp[-2]) + (ucell_t)tos);
	tos = POP(sp);
	NEXT();

	ASMLABEL(m_mul_code);
m_mul_code: /*: M* ( m_mul ) ;*/
	STORE_DOUBLE(&sp[-1], (dcell_t)tos * sp[-1]);
	sp++;
	NEXT();
