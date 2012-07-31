

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
unsigned int line;

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
        if(strcmp(label[nl],newline)==0) break;
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
int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int noff;
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

        for(ra=0;newline[ra];ra++) if(newline[ra]==':')
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
            fprintf(fpout,"label(%s);\n",newline);
            ret&=0x7FFFFFFF;
            if(globe[ret])
            {
                fprintf(fpout,"//push lr\n");
                fprintf(fpout,"TODO\n");
            }
            continue;
        }
        for(noff=0;newline[noff];noff++) if(newline[noff]!=' ') break;
        if(newline[noff]=='.')
        {
            if(strncmp(&newline[noff],".global",7)==0)
            {
                noff+=7;
                if(newline[noff]!=' ')
                {
                    fprintf(stderr,"<%u> syntax error\n",line);
                    return(1);
                }
                noff++;
                if((newline[noff]==0)||(newline[noff]==0))
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
                    fprintf(stderr,"<%u> label already defined\n",line);
                    return(1);
                }
            }
            continue;
        }




    }


    fclose(fpout);
    fclose(fpin);

    return(0);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
