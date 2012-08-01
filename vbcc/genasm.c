

//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLABELS 2048
#define LABELLEN 256
char label[MAXLABELS][LABELLEN];
char globe[MAXLABELS];
unsigned int nlabels;
unsigned int nl;


FILE *fpin;
FILE *fpout;

char newline[2048];
unsigned int noff;
unsigned int line;

unsigned int code[32];
unsigned int ncode;

unsigned int tcodes;

//(define-table p [
char p_name[0x20][8]=
{
"mov",
"cmn",
"add",
"bic",
"mul",
"eor",
"sub",
"and",
"mvn",
"ror",
"cmp",
"rsb",
"btst",
"or",
"extu",
"max",
"bset",
"min",
"bclr",
"adds2",
"bchg",
"adds4",
"adds8",
"adds16",
"exts",
"neg",
"lsr",
"clz",
"lsl",
"brev",
"asr",
"abs"
};

char q_name[0x10][8]=
{
"mov",
"add",
"mul",
"sub",
"mvn",
"cmp",
"btst",
"extu",
"bset",
"bclr",
"bchg",
"adds8",
"exts",
"lsr",
"lsl",
"asr"
};



char cc_name[0x10][4]=
{
"eq",
"ne",
"lo",
"hs",
"mi",
"pl",
"vs",
"vc",
"hi",
"ls",
"ge",
"lt",
"gt",
"le",
"",
"xx"
};


#define P_MOV 0x00
#define P_CMN 0x01
#define P_ADD 0x02
#define P_BIC 0x03

#define P_MUL 0x04
#define P_EOR 0x05
#define P_SUB 0x06
#define P_AND 0x07

#define P_MVN 0x08
#define P_ROR 0x09
#define P_CMP 0x0A
#define P_RSB 0x0B

#define P_BTST 0x0C
#define P_OR 0x0D
#define P_EXTU 0x0E
#define P_MAX 0x0F

#define P_BSET 0x10
#define P_MIN 0x11
#define P_BCLR 0x12
#define P_ADDS2 0x13

#define P_BCHG 0x14
#define P_ADDS4 0x15
#define P_ADDS8 0x16
#define P_ADDS16 0x17

#define P_EXTS 0x18
#define P_NEG 0x19
#define P_LSR 0x1A
#define P_CLZ 0x1B

#define P_LSL 0x1C
#define P_BREV 0x1D
#define P_ASR 0x1E
#define P_ABS 0x1F

#define P_SIZE 0x20

//(define-table q [
//"mov", "add", "mul", "sub",
//"mvn", "cmp", "btst", "extu",
//"bset", "bclr", "bchg", "adds8",
//"exts", "lsr", "lsl", "asr"])
#define Q_MOV 0x0
#define Q_ADD 0x1
#define Q_MUL 0x2
#define Q_SUB 0x3

#define Q_MVN 0x4
#define Q_CMP 0x5
#define Q_BTST 0x6
#define Q_EXTU 0x7

#define Q_BSET 0x8
#define Q_BCLR 0x9
#define Q_BCHG 0xA
#define Q_ADDS8 0xB

#define Q_EXTS 0xC
#define Q_LSR 0xD
#define Q_LSL 0xE
#define Q_ASR 0xF
#define Q_SIZE 0x10



#define CC_EQ 0x0
#define CC_NE 0x1
#define CC_LO 0x2
#define CC_HS 0x3
#define CC_MI 0x4
#define CC_PL 0x5
#define CC_VS 0x6
#define CC_VC 0x7
#define CC_HI 0x8
#define CC_LS 0x9
#define CC_GE 0xA
#define CC_LT 0xB
#define CC_GT 0xC
#define CC_LE 0xD
#define CC_UN 0xE
#define CC_XX 0xF


unsigned char numchar[256]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//-------------------------------------------------------------------
#include "diss.c"
//-------------------------------------------------------------------
void show_code ( void )
{
    unsigned int ra;

    if(ncode)
    {
        diss();
    }
    for(ra=0;ra<ncode;ra++)
    {
        fprintf(fpout,"emit1(0x%04X); ",code[ra]);
    }
    if(ncode)
    {
        fprintf(fpout,"\n");
        tcodes+=ncode;
    }
}
//-------------------------------------------------------------------
unsigned int p2q ( unsigned int p )
{
    switch(p)
    {
        case P_MOV  : return(Q_MOV  );
        case P_ADD  : return(Q_ADD  );
        case P_MUL  : return(Q_MUL  );
        case P_SUB  : return(Q_SUB  );
        case P_MVN  : return(Q_MVN  );
        case P_CMP  : return(Q_CMP  );
        case P_BTST : return(Q_BTST );
        case P_EXTU : return(Q_EXTU );
        case P_BSET : return(Q_BSET );
        case P_BCLR : return(Q_BCLR );
        case P_BCHG : return(Q_BCHG );
        case P_ADDS8: return(Q_ADDS8);
        case P_EXTS : return(Q_EXTS );
        case P_LSR  : return(Q_LSR  );
        case P_LSL  : return(Q_LSL  );
        case P_ASR  : return(Q_ASR  );
        //default: return(Q_SIZE);
    }
    return(Q_SIZE);
}
//-------------------------------------------------------------------
void gen_p2r ( unsigned int p, unsigned int rd, unsigned int rs )
{
    fprintf(fpout,"//optimize here\n");
    //1100 00pp pppd dddd   aaaa accc c00b bbbb      "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
    code[ncode++]=0xC000|(p<<5)|rd;
    code[ncode++]=0x0000|(rd<<11)|(CC_UN<<7)|rs;
}
//-------------------------------------------------------------------
void gen_p2rimm ( unsigned int p, unsigned int rd, unsigned int imm )
{
    fprintf(fpout,"//optimize here\n");
    //1110 10pp pppd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, #0x%08x{u}"
    code[ncode++]=0xE800|(p<<5)|rd;
    code[ncode++]=(imm>> 0)&0xFFFF;
    code[ncode++]=(imm>>16)&0xFFFF;
}
//-------------------------------------------------------------------
unsigned int check_label ( char *s )
{
    if(strlen(s)>=LABELLEN)
    {
        fprintf(stderr,"<%u> label too long\n",line);
        return(0xFFFFFFFF);
    }
    for(nl=0;nl<nlabels;nl++)
    {
        if(strcmp(label[nl],s)==0) break;
    }
    if(nl<nlabels)
    {
        return(nl);
    }
    if(nlabels>=MAXLABELS)
    {
        fprintf(stderr,"<%u> ran out of labels\n",line);
        return(0xFFFFFFFF);
    }
    fprintf(fpout,"declare(%s);\n",s);
    strcpy(label[nlabels],s);
    nl=nlabels; //should be the case already
    nlabels++;
    return(0x80000000+nl);
}
//-------------------------------------------------------------------
int parse_puigpr ( unsigned int p )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rd;
    unsigned int rs;
    unsigned int u;


    ra=noff;
    for(;newline[noff];noff++) if(newline[noff]==',') break;
    if(newline[noff]!=',')
    {
        fprintf(stderr,"<%u> syntax error\n",line);
        return(1);
    }
    newline[noff]=0;
    rd=strtoul(&newline[ra],NULL,10);
    if(rd>=32)
    {
        fprintf(stderr,"<%u> bad gpr\n",line);
        return(1);
    }
    //either a number or gpr
    noff++;
    if((newline[noff+0]=='g')&&(newline[noff+1]=='p')&&(newline[noff+2]=='r'))
    {
        noff+=3;
        ra=noff;
        for(;newline[noff];noff++)
        {
            if(newline[noff]==',') break;
            if(newline[noff]==' ') break;
        }
        if(newline[noff]==',')
        {
            fprintf(stderr,"<%u> three operand not supported yet\n",line);
            return(1);
        }
        if(newline[noff]==' ')
        {
            fprintf(stderr,"<%u> syntax error\n",line);
            return(1);
        }
        newline[noff]=0; //should be already
        //number check?
        rs=strtoul(&newline[ra],NULL,10);
        if(rs>=32)
        {
            fprintf(stderr,"<%u> bad gpr\n",line);
            return(1);
        }
        fprintf(fpout,"// %s r%u,r%u\n",p_name[p],rd,rs);
        ncode=0;
        gen_p2r(p,rd,rs);
        show_code();
    }
    else //is it a number
    {
        ra=noff;
        for(;newline[noff];noff++)
        {
            if(newline[noff]==',') break;
            if(newline[noff]==' ') break;
        }
        if(newline[noff]==',')
        {
            fprintf(stderr,"<%u> three operand not supported yet\n",line);
            return(1);
        }
        if(newline[noff]==' ')
        {
            fprintf(stderr,"<%u> syntax error\n",line);
            return(1);
        }
        newline[noff]=0; //should be already
        for(rb=ra;rb<noff;rb++) if(numchar[(int)newline[rb]]==0) break;
        if(rb<noff)
        {
            fprintf(stderr,"<%u> not a number, global variables not supported yet\n",line);
            return(1);
        }
        u=strtoul(&newline[ra],NULL,10);
        fprintf(fpout,"// %s gpr%u,0x%08X ; %u\n",p_name[p],rd,u,u);
        ncode=0;
        gen_p2rimm(p,rd,u);
        show_code();
    }
    return(0);
}
//-------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int ret;

    if(argc<2)
    {
        fprintf(stderr,".asm file not specified\n");
        return(1);
    }
    fpin=fopen(argv[1],"rt");
    if(fpin==NULL)
    {
        fprintf(stderr,"Error opening file [%s]\n",argv[1]);
        return(1);
    }
    sprintf(newline,"%s.c",argv[1]);
    fpout=fopen(newline,"wt");
    if(fpout==NULL)
    {
        fprintf(stderr,"Error creating file [%s]\n",newline);
        return(1);
    }



    memset(globe,0,sizeof(globe));
    nlabels=0;
    tcodes=0;

    fprintf(fpout,"#include \"tinyasm.c\"\n");
    fprintf(fpout,"START(\"%s.loader.bin\");\n",argv[1]);
    fprintf(fpout,"fillb(0x200, 0);\n");
    check_label("_notmain");
    fprintf(fpout,"b(_notmain);\n");
    //fprintf(fpout,"declare(hang);\n");
    //fprintf(fpout,"label(hang); //---------------------------\n");
    //fprintf(fpout,"b(hang);\n");

    //0x1F00 is branch to self...

    line=0;
    while(fgets(newline,sizeof(newline)-1,fpin))
    {
        line++;
        for(ra=0;newline[ra];ra++)
        {
            if(newline[ra]==0x09) newline[ra]=0x20;
            if(newline[ra]<0x20) newline[ra]=0;
        }
        fprintf(fpout,"//%s\n",newline);

        for(ra=0;newline[ra];ra++) if(newline[ra]==':') break;
        if(newline[ra]==':')
        {
            for(rb=0;rb<ra;rb++)
            {
                if(newline[rb]==' ')
                {
                    fprintf(stderr,"<%u> invalid label\n",line);
                    return(1);
                }
            }
            newline[ra]=0;
            ret=check_label(newline); if(ret==0xFFFFFFFF) return(1);
            fprintf(fpout,"label(%s); //---------------------------\n",newline);
            ret&=0x7FFFFFFF;
            if(globe[ret])
            {
                //fprintf(fpout,"//push r0,lr\n");
                //ncode=0;
                //code[ncode++]=0x0380;
                //show_code();

            }
            continue;
        }
        for(noff=0;newline[noff];noff++) if(newline[noff]!=' ') break;
        if(newline[noff]=='.')
        {
            if(strncmp(&newline[noff],".global ",8)==0)
            {
                noff+=8;
                if((newline[noff]==0)||(newline[noff]==' '))
                {
                    fprintf(stderr,"<%u> syntax error\n",line);
                    return(1);
                }
                ra=noff;
                for(rb=ra;newline[rb];rb++) if(newline[rb]==' ') break;
                newline[rb]=0;
                ret=check_label(&newline[ra]); if(ret==0xFFFFFFFF) return(1);
                if(ret&0x80000000)
                {
                    ret&=0x7FFFFFFF;
                    globe[ret]=1;
                }
                else
                {
                    ret&=0x7FFFFFFF;
                    globe[ret]=1;
                    //fprintf(stderr,"<%u> label already defined\n",line);
                    //return(1);
                }
            }
            continue;
        }
        if(newline[noff]=='#')
        {
            continue;
        }
        if(strcmp(&newline[noff],"rts")==0) //should not have anything after but null
        {
            //fprintf(fpout,"//pop r0,pc\n");
            //ncode=0;
            //code[ncode++]=0x0300;
            //show_code();
            fprintf(fpout,"//b lr\n");
            ncode=0;
            code[ncode++]=0x005A;
            show_code();
            continue;
        }
        if(strncmp(&newline[noff],"call ",5)==0)
        {
            noff+=5;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            ra=noff;
            for(rb=ra;newline[rb];rb++) if(newline[rb]==' ') break;
            newline[rb]=0;
            if(strcmp(&newline[ra],"_GET32")==0)
            {
                fprintf(fpout,"// ld r3,0(r3)\n");
                //ww=00 = 32 bit
                //0000 1ww0 ssss dddd         "; ld%s{w} r%i{d}, (r%i{s})"
                ncode=0;
                code[ncode++]=0x0833;
                show_code();
                continue;
            }
            if(strcmp(&newline[ra],"_PUT32")==0)
            {
                fprintf(fpout,"// st r4,0(r3)\n");
                //0000 1ww1 ssss dddd         "; st%s{w} r%i{d}, (r%i{s})"
                ncode=0;
                code[ncode++]=0x0934;
                show_code();
                continue;
            }
            if(strcmp(&newline[ra],"_PUT8")==0)
            {
                fprintf(fpout,"// strb r4,0(r3)\n");
                //0000 1ww1 ssss dddd         "; st%s{w} r%i{d}, (r%i{s})"
                ncode=0;
                code[ncode++]=0x0D34;
                show_code();
                continue;
            }
            if(strcmp(&newline[ra],"_dummy")==0)
            {
                continue;
            }
            if(strcmp(&newline[ra],"_ASMDELAY")==0)
            {
                //sub r3,1
                //011q qqqu uuuu dddd      "; %s{q} r%i{d}, #%i{u}"
                ncode=0;
                code[ncode++]=0x6000|(Q_SUB<<9)|(1<<4)|3;
                show_code();
                //cmp r3,0
                //011q qqqu uuuu dddd     "; %s{q} r%i{d}, #%i{u}"
                ncode=0;
                code[ncode++]=0x6000|(Q_CMP<<9)|(0<<4)|3;
                show_code();
                //bne -2
                //0001 1ccc cooo oooo  "; b%s{c} 0x%08x{$+o*2}"
                ncode=0;
                code[ncode++]=0x1800|(CC_NE<<7)|0x7E;
                show_code();
                continue;
            }
            ret=check_label(&newline[ra]); if(ret==0xFFFFFFFF) return(1);
            fprintf(fpout,"bl(%s);\n",&newline[ra]);
            continue;
        }
        //tst.ui gpr
        if(strncmp(&newline[noff],"tst.ui gpr",10)==0)
        {
            noff+=10;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            ra=noff;
            for(rb=ra;newline[rb];rb++) if(newline[rb]==' ') break;
            newline[rb]=0;
            ra=strtoul(&newline[ra],NULL,10);
            if(ra>=32)
            {
                fprintf(stderr,"<%u> bad gpr\n",line);
                return(1);
            }
            if(ra<16)
            {
                //011q qqqu uuuu dddd         "; %s{q} r%i{d}, #%i{u}"
                fprintf(fpout,"// cmp r%u,0\n",ra);
                ncode=0;
                code[ncode++]=0x6000|(Q_CMP<<9)|(0<<4)|ra;
                show_code();
            }
            else
            {
                //1100 00pp pppd dddd   aaaa accc c1ii iiii     "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
                fprintf(fpout,"// cmp r%u,0\n",ra);
                ncode=0;
                code[ncode++]=0xC000|(P_CMP<<5)|(ra);
                code[ncode++]=0x0040|(ra<<11)|(CC_UN<<7)|0;
                show_code();
            }
            continue;
        }
//static char *ccs[]={"eq","ne","lt","ge","le","gt",""};
        if(strncmp(&newline[noff],"beq ",4)==0)
        {
            noff+=4;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            ra=noff;
            for(rb=ra;newline[rb];rb++) if(newline[rb]==' ') break;
            newline[rb]=0;
            ret=check_label(&newline[ra]); if(ret==0xFFFFFFFF) return(1);
            fprintf(fpout,"beq(%s);\n",&newline[ra]);
            continue;
        }
        if(strncmp(&newline[noff],"bne ",4)==0)
        {
            noff+=4;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            ra=noff;
            for(rb=ra;newline[rb];rb++) if(newline[rb]==' ') break;
            newline[rb]=0;
            ret=check_label(&newline[ra]); if(ret==0xFFFFFFFF) return(1);
            fprintf(fpout,"bne(%s);\n",&newline[ra]);
            continue;
        }
        if(strncmp(&newline[noff],"blt ",4)==0)
        {
            noff+=4;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            ra=noff;
            for(rb=ra;newline[rb];rb++) if(newline[rb]==' ') break;
            newline[rb]=0;
            ret=check_label(&newline[ra]); if(ret==0xFFFFFFFF) return(1);
            fprintf(fpout,"blo(%s);\n",&newline[ra]);
            continue;
        }
        if(strncmp(&newline[noff],"b ",2)==0)
        {
            noff+=2;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            ra=noff;
            for(rb=ra;newline[rb];rb++) if(newline[rb]==' ') break;
            newline[rb]=0;
            ret=check_label(&newline[ra]); if(ret==0xFFFFFFFF) return(1);
            fprintf(fpout,"b(%s);\n",&newline[ra]);
            continue;
        }


        //mov.ui gpr
        if(strncmp(&newline[noff],"mov.ui gpr",10)==0)
        {
            noff+=10;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            parse_puigpr(P_MOV);
            continue;
        }
        //and.ui gpr
        if(strncmp(&newline[noff],"and.ui gpr",10)==0)
        {
            noff+=10;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            parse_puigpr(P_AND);
            continue;
        }
        //add.ui gpr
        if(strncmp(&newline[noff],"add.ui gpr",10)==0)
        {
            noff+=10;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            parse_puigpr(P_ADD);
            continue;
        }
        //sub.ui gpr
        if(strncmp(&newline[noff],"sub.ui gpr",10)==0)
        {
            noff+=10;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            parse_puigpr(P_SUB);
            continue;
        }
        //cmp.ui gpr
        if(strncmp(&newline[noff],"cmp.ui gpr",10)==0)
        {
            noff+=10;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            parse_puigpr(P_CMP);
            continue;
        }
        //or.ui gpr
        if(strncmp(&newline[noff],"or.ui gpr",9)==0)
        {
            noff+=9;
            if((newline[noff]==0)||(newline[noff]==' '))
            {
                fprintf(stderr,"<%u> syntax error\n",line);
                return(1);
            }
            parse_puigpr(P_OR);
            continue;
        }



        fprintf(fpout,".\n");
    }


    fprintf(fpout,"END\n");
    fprintf(stderr,"//tcodes 0x%X, %u\n",tcodes,tcodes);

    fclose(fpout);
    fclose(fpin);

    return(0);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
