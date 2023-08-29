#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

static long double stack[1024];
static int s = 0;

#define BARF(fmt, ...)		do { fprintf(stderr, fmt "\n", ##__VA_ARGS__); exit(EXIT_FAILURE); } while (0)

#define PUSH(v) stack[++s] = (v)
#define POP stack[s--]
#define PEEK(a) stack[s-a]
#define SWP(a, b) { PEEK(-1)=PEEK(a); PEEK(a)=PEEK(b); PEEK(b)=PEEK(-1); }

int
main(int argc, char **argv)
{
	long double reg['z'-'a'+1] = { 0 };
	reg['e'-'a'] = M_E;
	reg['p'-'a'] = M_PI;

	char *buf = NULL;
	size_t len = 0;
	
	for (ssize_t r; (r = getline(&buf, &len, stdin)) != -1;) {
		long double v = 0;
		buf[r-1] = '\0';
		switch (buf[0]) {
		case '.':	if (!isdigit(buf[1])) break;
		if(0){
		case '+':
		case '-':	if (!isdigit(buf[1]) && !(buf[1]=='.' && isdigit(buf[2]))) break;
		}
		case '0'...'9':	PUSH(atof(buf)); continue;
		}
		switch (buf[0]) {
		case '+':		v=POP+POP;	break;
		case '-': SWP(0,1);	v=POP-POP;	break;
		case '*': 		v=POP*POP;	break;
		case '/': SWP(0,1);	v=POP/POP;	break;
		case '%': SWP(0,1);	v=(int)POP % (int)POP;	break;
		case '^': 		v=pow(POP, POP);	break;
		case '\\':SWP(0,1);	v=log(POP)/log(POP);	break;
		case '!':	if (PEEK(0)<0) BARF("bad %Lf",PEEK(0));
				v=1; for (int i=(int)POP; i>1; i--) v=v*i;
				break;
		case '>': SWP(0,1);	v=POP > POP ?1:0;	break;
		case '<': SWP(0,1);	v=POP < POP ?1:0;	break;
		case 'a'...'z':		v=reg[buf[0]-'a'];	break;
		case '=':	if (buf[1]>='a' && buf[1]<='z') {
					reg[buf[1]-'a'] = POP;
					continue;
				}
				v=POP == POP ?1:0;		break;
		case '.':		v=PEEK(0);		break;
		case ',':	SWP(0,1); continue;
		case '\0':	for (int i=0; i<s; i++) printf("  %Lf\n",stack[i+1]);
				continue;
		default:	fprintf(stderr, "?\n"); continue;
		}
		PUSH(v);
	}
	free(buf);
}
