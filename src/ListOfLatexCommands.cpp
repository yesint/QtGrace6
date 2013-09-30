/***************************************************************************
 *   Copyright (C) 2008-2012 by Andreas Winter                             *
 *   andreas.f.winter@web.de                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cstdio>
#include <iostream>
#include <cstring>
#include <QtGui>
#include "defines.h"
#include "t1fonts.h"
#include "utils.h"
#define toAscii toLatin1
using namespace std;

extern char dummy[];

static int font_nr;
static double oblique, vshift, hshift, zoom;
static bool underline,overline;
QString errmessage;

struct LatexCommands
{
    char * la_com;
    char font;
    char * ch;
};

struct SpecialLatexCommands
{
    char * la_com;
    char * gr_com;
    char * stop_com;
    int nr_of_exp_args;
};

#define NUMBER_OF_LATEX_COMMANDS 78

struct LatexCommands allCommands[NUMBER_OF_LATEX_COMMANDS]=
{
    //0
{"alpha",'x',"a"},
{"beta",'x',"b"},
{"gamma",'x',"g"},
{"delta",'x',"d"},
{"epsilon",'x',"\\cN\\C"},
{"varepsilon",'x',"e"},
{"zeta",'x',"z"},
{"eta",'x',"h"},
{"theta",'x',"q"},
{"vartheta",'x',"J"},
//10
{"iota",'x',"i"},
{"kappa",'x',"k"},
{"lambda",'x',"l"},
{"mu",'x',"m"},
{"nu",'x',"n"},
{"xi",'x',"x"},
{"pi",'x',"p"},
{"varpi",'x',"v"},
{"rho",'x',"r"},
{"varrho",'x',"r"},//not exactely the same...
//20
{"sigma",'x',"s"},
{"varsigma",'x',"V"},
{"tau",'x',"t"},
{"upsilon",'x',"u"},
{"phi",'x',"f"},
{"varphi",'x',"j"},
{"chi",'x',"c"},
{"psi",'x',"y"},
{"omega",'x',"w"},
{"Gamma",'x',"G"},
//30
{"Delta",'x',"D"},
{"Theta",'x',"Q"},
{"Lambda",'x',"L"},
{"Xi",'x',"X"},
{"Pi",'x',"P"},
{"Sigma",'x',"S"},
//{"Upsilon",'x',"\\c!\\C"},
{"Phi",'x',"F"},
{"Psi",'x',"Y"},
{"Omega",'x',"W"},
//40
{"sum",'x',"\\ce\\C"},
{"prod",'x',"\\cU\\C"},
{"int",'x',"\\cr\\C"},
{"in",'x',"\\cN\\C"},
{"forall",'x',"\""},
{"exists",'x',"$"},
{"surd",'x',"\\cV\\C"},
{"Rightarrow",'x',"\\c^\\C"},
{"Leftarrow",'x',"\\c\\\\\\C"},
{"propto",'x',"\\c5\\C"},
//50
{"partial",'x',"\\c6\\C"},
{"infty",'x',"\\c%\\C"},
{"cdot",'x',"\\cW\\C"},
{"times",'x',"\\c4\\C"},
{"pm",'x',"\\c1\\C"},
{"equiv",'x',"\\c:\\C"},
{"approx",'x',"\\c;\\C"},
{"ast",'x',"*"},
{"bullet",'x',"\\c7\\C"},
{"div",'x',"\\c8\\C"},
//60
{"nabla",'x',"\\cQ\\C"},
{"bot",'x',"^"},
{"clubsuit",'x',"\\c'\\C"},
{"diamondsuit",'x',"\\c(\\C"},
{"heartsuit",'x',"\\c)\\C"},
{"spadesuit",'x',"\\c*\\C"},
{"rightarrow",'x',"\\c.\\C"},
{"leftarrow",'x',"\\c,\\C"},
{"leq",'x',"\\c#\\C"},
{"geq",'x',"\\c3\\C"},
//70
{"angle",'x',"\\cP\\C"},
{"Re",'x',"\\cB\\C"},
{"Im",'x',"\\cA\\C"},
{"neq",'x',"\\c9\\C"},
{"sim",'x',"~"},
//  {"hbar",'1',"h\\h{-0.55}\\v{-0.04}\\c/\\C\\h{0.3}\\v{0.04}"},
{"oint",'x',"\\h{0.1}\\cr\\C\\f{Courier}\\h{-0.425}\\v{0.2}o\\v{-0.2}"},
//  {"odot",'x',"\\h{0.2}\\v{0.05}\\cW\\C\\f{Courier}\\h{-0.435}\\v{0.055}o\\v{-0.105}"},
{"circ",'x',"o"},
{"leftrightarrow",'x',"\\c+\\C"},
//80
{"Leftrightarrow",'x',"\\c[\\C"},
// {"AA",'0',"\\cE\\C"}
};

#define NUMBER_OF_SPECIAL_LATEX_COMMANDS 6

struct SpecialLatexCommands specCommands[NUMBER_OF_SPECIAL_LATEX_COMMANDS]=
{
    //0
{"^","\\S","\\N",1},
{"_","\\s","\\N",1},
{"overline","\\o","\\O",1},
{"underline","\\u","\\U",1},
{"sqrt","\\x\\cV\\C\\0\\o","\\O",1},
{"frac","\\v{0.5}","\\N",2}

};

int replace_command(char * command,char & new_font)
{
    new_font=-1;//invalid
    for (int i=0;i<NUMBER_OF_LATEX_COMMANDS;i++)
    {
        if (!strcmp(command,allCommands[i].la_com))
        {
            strcpy(command,allCommands[i].ch);
            command[strlen(allCommands[i].ch)]='\0';
            new_font=allCommands[i].font;
            break;
        }
    }
    if (new_font==-1)
        return RETURN_FAILURE;
    else
        return RETURN_SUCCESS;
}

void remove_emptys(char * text)
{
    if (text[0]!=' ' && text[strlen(text)-1]!=' ') return;
    int pos=0;
    char * dummy=new char[strlen(text)];
    while (pos<strlen(text) && isspace(text[pos])) pos++;
    strcpy(dummy,text+pos);
    pos=strlen(dummy)-1;
    while (pos>0 && isspace(dummy[pos])) pos--;
    if (!isspace(dummy[pos])) pos++;
    strcpy(text,dummy);//pos is now the position of the last character that is not ' '
    text[pos]='\0';
    delete[] dummy;
}

int remove_braces(char * text)//removes {} at beginning and end if they belong together! returns the position of the closing brace to the initially opened one (returning 0 means: {} removed; returning -1 means: no {} found)
{
    int len=strlen(text);
    if (text[0]!='{' || text[len-1]!='}') return -1;
    int bracecounter=1;//first brace
    int i;
    for (i=1;i<len-1;i++)//do not look at first or last character since we know they are braces
    {
        if (text[i]=='{') bracecounter++;
        if (text[i]=='}') bracecounter--;
        if (bracecounter==0) break;//the initially opend brace has been closed!
    }
    if (i<len-1)
    {
        return i;
    }
    else
    {
        char * dummy=new char[len];
        strcpy(dummy,text+1);
        strcpy(text,dummy);
        text[len-2]='\0';
        delete[] dummy;
    }
    return 0;
}

void arg_splitter(char * argument,char * argument1,char * argument2)
{
    argument1[0]=argument2[0]='\0';
    remove_emptys(argument);
    int ret=remove_braces(argument);
    while (ret==0)//continue removing unused braces
        ret=remove_braces(argument);
    if (ret==-1) return;//no two arguments
    //two arguments
    strcpy(argument1,argument+1);
    argument1[ret-1]='\0';
    strcpy(argument2,argument+ret+1);
    ret=remove_braces(argument1);
    while (ret==0)//continue removing unused braces
        ret=remove_braces(argument1);
    ret=remove_braces(argument2);
    while (ret==0)//continue removing unused braces
        ret=remove_braces(argument2);
}

void find_first_command(char * text,int & start_command,int & command_length,int * brace_pos)
{
    int len,i,bracecounter;
    len=strlen(text);
    bracecounter=0;
    start_command=-1;
    command_length=0;
    brace_pos[0]=brace_pos[1]=brace_pos[2]=brace_pos[3]=-1;
    for (i=0;i<len;i++)
    {
        if (text[i]=='\\' || text[i]=='_' || text[i]=='^')//beginning of a command
        {
            start_command=i;
            break;
        }
    }
    if (start_command!=-1)
    {
        if (text[start_command]=='\\')//we found a real command
        {
            for (i=start_command+1;i<=len;i++)
            {
                if (text[i]=='\\' || text[i]==' ' || text[i]=='=' || text[i]==')' || text[i]=='(' || text[i]=='{' || text[i]=='_' || text[i]=='^' || text[i]=='\0')
                {
                    command_length=i;//at the moment: this is the position of the first character after the command, not the length
                    break;
                }
            }
            if (text[command_length]=='{')
            {//brace found
                brace_pos[0]=command_length;
                bracecounter=1;
                for (i=brace_pos[0]+1;i<len;i++)
                {
                    if (text[i]=='{') bracecounter++;
                    if (text[i]=='}') bracecounter--;
                    if (text[i]=='}' && bracecounter==0)
                    {
                        brace_pos[1]=i;
                        break;
                    }
                }//hopefully: closing brace found
                if (brace_pos[1]<len-1)//not the last character
                {
                    if (text[brace_pos[1]+1]=='{')//another brace found
                    {
                        brace_pos[2]=brace_pos[1]+1;
                        bracecounter=1;
                        for (i=brace_pos[2]+1;i<len;i++)
                        {
                            if (text[i]=='{') bracecounter++;
                            if (text[i]=='}') bracecounter--;
                            if (text[i]=='}' && bracecounter==0)
                            {
                                brace_pos[3]=i;
                                break;
                            }
                        }
                    }//now we should have found 4 braces
                }
            }//end brace found
            command_length-=start_command+1;//now it is the length of the command
        }
        else//we found '^' or '_'
        {
            command_length=1;
            brace_pos[0]=start_command+1;//may not actually be a brace
            bracecounter=0;
            if (text[brace_pos[0]]!='\\' && text[brace_pos[0]]!='{')//regular text --> only one character counts
            {
                brace_pos[1]=brace_pos[0]+1;
            }
            else
            {
                for (i=start_command+1;i<=len;i++)
                {
                    if (text[i]=='\0')
                    {
                        brace_pos[1]=i;
                        break;
                    }
                    if (text[i]=='{') bracecounter++;
                    if (text[i]=='}') bracecounter--;
                    if (bracecounter==0 && i<len-1 && text[i]=='}' && text[i+1]=='{')
                    {
                        bracecounter++;
                        i++;//we already counted the next one
                        continue;
                    }
                    if ((text[i]=='}' || text[i]==' ') && bracecounter==0)
                    {
                        brace_pos[1]=i;
                        break;
                    }
                }
            }
        }
    }//end command found
}

void splitter(char * text,int start_command,int command_length,int * brace_pos,char * left_part,char * middle_part,char * argument_part,char * right_part)
{
    static int first_brace,last_brace;
    first_brace=last_brace=brace_pos[0];
    if (first_brace!=-1)
        last_brace=brace_pos[1];
    if (brace_pos[3]!=-1)
        last_brace=brace_pos[3];
    strcpy(left_part,text);
    argument_part[0]=middle_part[0]=right_part[0]='\0';
    if (start_command!=-1)
    {
        left_part[start_command]='\0';
        if (text[start_command]=='\\')
            strcpy(middle_part,text+start_command+1);
        else
            strcpy(middle_part,text+start_command);
        middle_part[command_length]='\0';
        if (first_brace!=-1)
        {
            strcpy(argument_part,text+first_brace);
            if (text[first_brace]=='{')
            {
                argument_part[last_brace-first_brace+1]='\0';
                strcpy(right_part,text+last_brace+1);
            }
            else
            {
                argument_part[last_brace-first_brace]='\0';
                strcpy(right_part,text+last_brace);
            }
        }
        else
            strcpy(right_part,text+start_command+1+command_length);
    }
}

int find_spec_function(char * command)
{
    static int ret;
    ret=-1;
    for (int i=0;i<NUMBER_OF_SPECIAL_LATEX_COMMANDS;i++)
    {
        if (!strcmp(specCommands[i].la_com,command))
        {
            ret=i;
            break;
        }
    }
    return ret;
}

int recursive_replacer(char * text)
{//takes text and searches for the first latex-command beginning with '\' or '^' or '_' and replaces this command
    //if '{' is encountered, the contents of the '{}' are given to another instance of this function
    //if no command is found, the text is returned unchanged, the same is the case if the command can not be identified
    //if no '{' is found the function continues to replace the following commands or copies the following text
    //returns the number of actual commands found
    int command_count=0;
    int len=strlen(text);


    if (len==0) return 0;

    char new_font;
    char * result=new char[256+len];
    char * temp=new char[256+len];
    //cout << "input=#" << text << "#" << endl;
    char * left_part=new char[128+len];
    char * middle_part=new char[128+len];
    char * argument_part=new char[128+len];
    char * argument1=new char[128+len];
    char * argument2=new char[128+len];
    char * right_part=new char[128+len];

    int start_command=0,command_length,brace_pos[4];
    int res_pos=0,com_nr;

    remove_emptys(text);
    //cout << "after remove input=#" << text << "#" << endl;
    if (strlen(text)>0)
    {
        strcpy(temp,text);
        while (start_command!=-1 && strlen(temp))
        {
            find_first_command(temp,start_command,command_length,brace_pos);
            splitter(temp,start_command,command_length,brace_pos,left_part,middle_part,argument_part,right_part);
            //cout << "pos=" << start_command << " len=" << command_length << " " << middle_part << endl;
            strcpy(result+res_pos,left_part);//copy the part that does not contain any useful commands
            res_pos+=strlen(left_part);
            command_count+=strlen(left_part);//we have to count normal characters as commands --> necessary to count length of commands for complicated commands
            arg_splitter(argument_part,argument1,argument2);//split this to be on the save side!
            //cout << "#" << left_part << "#" << middle_part << "#" << argument_part << "#" << right_part << "#" << endl;
            if (strlen(middle_part)>0)
                if (replace_command(middle_part,new_font)==RETURN_SUCCESS)
                {
                    //cout << "new_font=" << new_font << endl;
                    command_count++;
                    if (new_font!=-1)
                    {
                        result[res_pos++]='\\';
                        result[res_pos++]='f';
                        result[res_pos++]='{';
                        if (new_font=='x')
                        {
                            result[res_pos++]='S';
                            result[res_pos++]='y';
                            result[res_pos++]='m';
                            result[res_pos++]='b';
                            result[res_pos++]='o';
                            result[res_pos++]='l';
                        }
                        else
                            result[res_pos++]=new_font;
                        result[res_pos++]='}';
                    }
                    strcpy(result+res_pos,middle_part);
                    res_pos+=strlen(middle_part);
                    if (new_font!=-1)
                    {
                        result[res_pos++]='\\';//we set back to standard font
                        result[res_pos++]='f';
                        result[res_pos++]='{';
                        result[res_pos++]='}';
                    }
                    //cout << "result=" << result << endl;
                }
                else
                {//unrecognized command may be a special command
                    com_nr=find_spec_function(middle_part);
                    //cout << "#special command#" << middle_part << "# com_nr=" << com_nr << endl;
                    if (com_nr!=-1)//special command recognized
                    {
                        command_count++;
                        strcpy(result+res_pos,specCommands[com_nr].gr_com);
                        res_pos+=strlen(specCommands[com_nr].gr_com);
                        if (specCommands[com_nr].nr_of_exp_args==1)
                        {
                            command_count+=recursive_replacer(argument_part);
                            strcpy(result+res_pos,argument_part);
                            res_pos+=strlen(argument_part);
                        }
                        else
                        {
                            if (com_nr==5 && argument1[0]!='\0')//special case 'frac'
                            {

                                int count1,count2,max_count;
                                count1=count2=0;
                                count1=recursive_replacer(argument1);
                                count2=recursive_replacer(argument2);
                                max_count=count1>count2?count1:count2;
                                //cout << "frac found " << count1 << " " << count2 << " max=" << max_count << endl;
                                if (count1>=count2)
                                {
                                    strcpy(dummy,"\\u");
                                }
                                else
                                {//lower part is longer
                                    sprintf(dummy,"\\h{%f}",(count2-count1)*0.25);
                                }
                                strcpy(result+res_pos,dummy);//commands to go on top of the line
                                res_pos+=strlen(dummy);
                                strcpy(result+res_pos,argument1);//copy upper part = first argument of frac
                                res_pos+=strlen(argument1);
                                if (count1>=count2)
                                {
                                    sprintf(dummy,"\\U\\v{-1}\\h{%f}",-count1*0.5+0.25*(count1-count2));//switch from above to below line
                                }
                                else
                                {
                                    sprintf(dummy,"\\v{-1}\\h{%f}\\o",0.25*(count1-count2)-0.5*count1);
                                }
                                strcpy(result+res_pos,dummy);//go below line
                                res_pos+=strlen(dummy);
                                strcpy(result+res_pos,argument2);//copy lower part = second argument of frac
                                res_pos+=strlen(argument2);
                                if (count1>=count2)
                                {
                                    sprintf(dummy,"\\v{0.5}\\h{%f}",(count1-count2)*0.25);
                                }
                                else
                                {
                                    sprintf(dummy,"\\O\\v{0.5}");
                                }
                                strcpy(result+res_pos,dummy);//end command
                                res_pos+=strlen(dummy);

                                errmessage=QObject::tr("Warning:frac currently not supported by pdf, png, and svg export files");
                                errmsg(errmessage.toAscii().constData());
                            }
                            else
                            {
                                errmessage=QObject::tr("command too complicated: #") + QString(middle_part) + QObject::tr("#\n") + QObject::tr("argument_part=#") + QString(argument_part) + QObject::tr("#argument1=#") + QString(argument1) + QObject::tr("#argument2=#") + QString(argument2);
                                errmsg(errmessage.toAscii().constData());
                                //cout << "command too complicated: #" << middle_part << "#" << endl;
                                //cout << "argument_part=#" << argument_part << "#argument1=#" << argument1 << "#argument2=#" << argument2 << endl;
                            }
                        }
                        strcpy(result+res_pos,specCommands[com_nr].stop_com);
                        res_pos+=strlen(specCommands[com_nr].stop_com);

                        if(!strcmp("overline",middle_part)||!strcmp("sqrt",middle_part)||!strcmp("underline",middle_part)||!strcmp("frac",middle_part)){
                            errmessage=QObject::tr("Warning:" )+ QString(middle_part) + QObject::tr(" currently not supported by pdf, png, and svg export files");
                            errmsg(errmessage.toAscii().constData());
                        }

                    }
                    else
                    {
                        errmessage=QObject::tr("special command not found: #") + QString(middle_part) + QObject::tr("#");
                        //cout << "special command not found: #" << middle_part << "#" << endl;
                        errmsg(errmessage.toAscii().constData());
                        strcpy(result+res_pos,middle_part);//just copy the command...
                        res_pos+=strlen(middle_part);
                        command_count+=strlen(middle_part);
                    }
                }
            command_count+=recursive_replacer(right_part);//right part is clear with this
            strcpy(result+res_pos,right_part);//go on with right part
            res_pos+=strlen(right_part);
            //cout << "temp=#" << temp << "#" << endl;
            temp[0]='\0';
        }//end of while-loop
        //cout << "result=#" << result << "#" << endl;
        strcpy(text,result);
    }//end test for empty 'text'

    delete[] left_part;
    delete[] middle_part;
    delete[] argument_part;
    delete[] argument1;
    delete[] argument2;
    delete[] right_part;
    delete[] result;
    delete[] temp;

    return command_count;
}

void last_settings_in_string(QString text,int & font,double & oblique,double & vshift,double & hshift,double & zoom,bool & underline,bool & overline)
{
    static QRegExp regex_f1("\\\\f\\{");//should find \f{
    static QRegExp regex_f2("\\\\\\d");//should find \0...\9
    static QRegExp regex_obl1("\\q");
    static QRegExp regex_obl2("\\Q");
    static QRegExp regex_obl3("\\\\l\\{");
    static QRegExp regex_vshift1("\\\\v\\{");
    static QRegExp regex_hshift1("\\\\h\\{");
    static QRegExp regex_zoom1("\\\\\\+");
    static QRegExp regex_zoom2("\\\\\\-");
    static QRegExp regex_zoom3("\\\\z\\{");
    static QRegExp regex_ul1("\\\\u");
    static QRegExp regex_ul2("\\\\U");
    static QRegExp regex_ol1("\\\\o");
    static QRegExp regex_ol2("\\\\O");


    static QString result,mtext;
    static char mtext2[32];
    static int pos1,pos2,pos3;

    result=text;
    ///font=-1;//original font --> \f{}

    //First part: find font type
    pos1=pos2=pos3=0;
    while ((pos1=regex_f1.indexIn(result,pos1))>=0 || (pos2=regex_f2.indexIn(result,pos2))>=0)
    {
        if (pos1>=0)//long font-nr like \f{Symbol}
        {
            pos2=result.indexOf(QString("}"),pos1+1);
            mtext=result.mid(pos1+3,pos2-pos1-3);
            //cout << "font mtext=#" << mtext.toAscii().constData() << "#" << endl;
            if (mtext.length()<=0)
                font=-1;
            else
            {
                strcpy(mtext2,mtext.toAscii());
                font=get_font_by_name(mtext2);
            }
            pos1=pos2=pos2+1;
        }
        else//short font-nr like \5
        {
            mtext2[0]=result.at(pos2+1).toAscii();
            mtext2[1]='\0';
            font=atoi(mtext2);
            //cout << "mfont=" << font << endl;
            pos1=pos2=pos2+2;
        }
    }
    //now font should be known

    //Second part: find oblique setting
    pos1=pos2=pos3=0;
    while ((pos1=regex_obl1.indexIn(result,pos1))>=0 || (pos2=regex_obl2.indexIn(result,pos2))>=0 || (pos3=regex_obl3.indexIn(result,pos3))>=0)
    {
        if (pos1>=0)//q-->+0.25
        {
            oblique+=0.25;
            pos1=pos2=pos3=pos1+2;
        }
        else if (pos2>=0)//Q-->-0.25
        {
            oblique-=0.25;
            pos1=pos2=pos3=pos2+2;
        }
        else// \l{x}
        {
            pos2=result.indexOf(QString("}"),pos3+1);
            mtext=result.mid(pos3+3,pos2-pos3-3);
            //cout << "oblique mtext=#" << mtext.toAscii().constData() << "#" << endl;
            if (mtext.length()>0)
            {
                strcpy(mtext2,mtext.toAscii());
                oblique+=atof(mtext2);
            }
            pos1=pos2=pos3=pos2+1;
        }
    }
    //now we should know the oblique setting

    //Third part: find shifts
    pos1=pos2=pos3=0;
    while ((pos1=regex_vshift1.indexIn(result,pos1))>=0 || (pos2=regex_hshift1.indexIn(result,pos2))>=0)
    {
        if (pos1>=0)//vshift
        {
            pos2=result.indexOf(QString("}"),pos1+1);
            mtext=result.mid(pos1+3,pos2-pos1-3);
            //cout << "vshift mtext=#" << mtext.toAscii().constData() << "#" << endl;
            if (mtext.length()>0)
            {
                strcpy(mtext2,mtext.toAscii());
                vshift+=atof(mtext2);
            }
            pos1=pos2=pos3=pos2+1;
        }
        else//hshift
        {
            pos3=result.indexOf(QString("}"),pos2+1);
            mtext=result.mid(pos2+3,pos3-pos2-3);
            //cout << "hshift mtext=#" << mtext.toAscii().constData() << "#" << endl;
            if (mtext.length()>0)
            {
                strcpy(mtext2,mtext.toAscii());
                hshift+=atof(mtext2);
            }
            pos1=pos2=pos3=pos3+1;
        }
    }
    //now we should know the actual shifts

    //Fourth part: find zoom factor
    pos1=pos2=pos3=0;
    while ((pos1=regex_zoom1.indexIn(result,pos1))>=0 || (pos2=regex_zoom2.indexIn(result,pos2))>=0 || (pos3=regex_zoom3.indexIn(result,pos3))>=0)
    {
        if (pos1>=0)// \+ --> \z{1.19} --> *1.19
        {
            zoom*=1.19;
            pos1=pos2=pos3=pos1+2;
        }
        else if (pos2>=0)// \- --> \z{0.84} --> *0.84
        {
            zoom*=0.84;
            pos1=pos2=pos3=pos2+2;
        }
        else// \z{} or \z{x}
        {
            pos2=result.indexOf(QString("}"),pos3+1);
            mtext=result.mid(pos3+3,pos2-pos3-3);
            //cout << "zoom mtext=#" << mtext.toAscii().constData() << "#" << endl;
            if (mtext.length()>0)
            {
                strcpy(mtext2,mtext.toAscii());
                zoom*=atof(mtext2);
            }
            else
                zoom=1.0;
            pos1=pos2=pos3=pos2+1;
        }
    }
    //now we should know the zoom factor

    //Fifth part: find underline
    pos1=pos2=pos3=0;
    while ((pos1=regex_ul1.indexIn(result,pos1))>=0 || (pos2=regex_ul2.indexIn(result,pos2))>=0)
    {
        if (pos1>=0)// \u
        {
            underline=true;
            pos1=pos2=pos3=pos1+2;
        }
        else// \U
        {
            underline=false;
            pos1=pos2=pos3=pos2+2;
        }
    }
    //now we should know wether underline is present

    //Sixth part: find overline
    pos1=pos2=pos3=0;
    while ((pos1=regex_ol1.indexIn(result,pos1))>=0 || (pos2=regex_ol2.indexIn(result,pos2))>=0)
    {
        if (pos1>=0)// \o
        {
            overline=true;
            pos1=pos2=pos3=pos1+2;
        }
        else// \O
        {
            overline=false;
            pos1=pos2=pos3=pos2+2;
        }
    }
    //now we should know wether overline is present
}

void include_spec_text_settings(QString & text,int & font,double & oblique,double & vshift,double & hshift,double & zoom,bool & underline,bool & overline,bool minimal)
{
    char dummy[512];
    text=QString("");

    if (font==-1)
        sprintf(dummy,"\\f{}");//switch to Std-font
    else
        sprintf(dummy,"\\f{%d}",font);
    text+=QString(dummy);
    if (minimal==false)
    {

        if (oblique!=0.0)
        {
            sprintf(dummy,"\\l{%f}",oblique);
            text+=QString(dummy);
        }
        if (vshift!=0.0)
        {
            sprintf(dummy,"\\v{%f}",vshift);
            text+=QString(dummy);
        }
        if (hshift!=0.0)
        {
            sprintf(dummy,"\\h{%f}",hshift);
            text+=QString(dummy);
        }
        if (zoom!=1.0)
        {
            sprintf(dummy,"\\z{%f}",zoom);
            text+=QString(dummy);
        }

    }//end minimal==false

    if (underline==true)
        text+=QString("\\u");
    if (overline==true)
        text+=QString("\\o");
}

void complete_LaTeX_to_Grace_Translator(QString & text)
{
    static int pos,pos2,altpos,ret;
    static QString str,str2;
    static QString ref("$$");
    static QString result,intermediate,new_text;
    static QString text_before,text_incl_options;
    ///cout << "to Convert #" << text.toAscii().constData() << "#-->#";
    //initialization
    underline=overline=false;
    font_nr=-1;//no special font --> \f{}
    vshift=hshift=oblique=0.0;
    zoom=1.0;
    //font_nr=0;//no special font
    str=result=text;
    pos=0;
    pos2=0;
    altpos=-1;
    while ((pos=str.indexOf(ref,pos))>=0)
    {
        //cout << "str=" << str.toAscii().constData() << " ZZZZ=" << zoom << endl;
        if (pos==altpos) break;
        //cout << "pos=" << pos << " pos2=" << pos2 << endl;
        altpos=pos;
        //now pos=$$ and pos2=first normal text position (maybe the beginning or the first text after the last closing '$$')
        text_before=str.mid(pos2,pos-pos2);
        last_settings_in_string(text_before,font_nr,oblique,vshift,hshift,zoom,underline,overline);
        include_spec_text_settings(text_incl_options,font_nr,oblique,vshift,hshift,zoom,underline,overline,true);
        /// text_incl_options contains apart from the font only special text-commands like underline, overline or oblique
        //cout << "text before #" << text_before.toAscii().constData() << "#" << " ZZZZ=" << zoom<< endl;
        //cout << "text incl. options #" << text_incl_options.toAscii().constData() << "#" << endl;
        pos+=2;//beginning of commands after '$$'
        pos2=str.indexOf(ref,pos);//search for end of command
        str2=str.mid(pos,pos2-pos);
        intermediate=ref+str2+ref;//The complete LaTeX-command framed by '$$'
        //cout << "intermediate #" << intermediate.toAscii().constData() << "#" << endl;
        strcpy(dummy,str2.toAscii());//dummy is now the LaTeX-command without the '$$'
        //cout << "without dollar #" << dummy << "#" << endl;
        ret=recursive_replacer(dummy);//everything will be replaced by Grace-commands here
        new_text=QString(dummy)+text_incl_options;
        //result.replace(intermediate,QString(dummy)+text_incl_options);//replace commands in actual string
        result.replace(pos-2,intermediate.length(),new_text);//replace only one command
        pos-=2;//beginning first '$$' in original text = first position that has been replaced
        //pos+=strlen(dummy);//this should be the first character after the replacement --> continue search here
        pos+=new_text.length();
        //pos+=text_incl_options.length();
        str=result;
        //cout << "str=#" << str.toAscii().constData() << "# result=#" << result.toAscii().constData() << "#" << " ZZZZ=" << zoom<< endl;
    }
    ///cout << result.toAscii().constData() << "#" << endl;
    text=result;
}

