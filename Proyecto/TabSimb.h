
/* Function type. */
typedef double (func_t) (double);

/* Data type for links in the chain of symbols. */
struct symrec {
	char *name;  /* name of symbol */
    int type;    /* type of symbol:  VAR(int) 0 or Strin_Literal */
    union {
        int var;    /* value of a entero */
    	char *cadena;   /* value of a cadena */
    } value;
    char *VarDir;
	struct symrec *next;  /* link field */
};

typedef struct symrec symrec;

/* The symbol table: a chain of 'struct symrec'. */
extern symrec *sym_table;

symrec *putsym (char const *name, int sym_type);
symrec *getsym (char const *name);
