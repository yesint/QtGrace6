/***************************************************************************
 *   Copyright (C) 2008-2022 by Andreas Winter                             *
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

using namespace std;

extern char dummy[];
extern bool useQtFonts;

static int font_nr;
static double oblique, vshift, hshift, zoom;
static bool underline,overline;

struct SpecialLatexCommands
{
    const char * la_com;
    const char * gr_com;
    const char * stop_com;
    int nr_of_exp_args;
};

#define NUMBER_OF_LATEX_COMMANDS 82

struct LatexCommands allCommands[NUMBER_OF_LATEX_COMMANDS]=
{
//0
{"alpha",'x',"a",QString("α"),945},
{"beta",'x',"b",QString("β"),946},
{"gamma",'x',"g",QString("γ"),947},
{"delta",'x',"d",QString("δ"),948},
{"epsilon",'x',"\\cN\\C",QString("ϵ"),1013},
{"varepsilon",'x',"e",QString("ε"),949},
{"zeta",'x',"z",QString("ζ"),950},
{"eta",'x',"h",QString("η"),951},
{"theta",'x',"q",QString("θ"),952},
{"vartheta",'x',"J",QString("ϑ"),977},
//10
{"iota",'x',"i",QString("ι"),953},
{"kappa",'x',"k",QString("κ"),954},
{"lambda",'x',"l",QString("λ"),955},
{"mu",'x',"m",QString("μ"),956},
{"nu",'x',"n",QString("ν"),957},
{"xi",'x',"x",QString("ξ"),958},
{"pi",'x',"p",QString("π"),960},
{"varpi",'x',"v",QString("ϖ"),982},
{"rho",'x',"r",QString("ρ"),961},
{"varrho",'x',"r",QString("ϱ"),1009},//not exactely the same in iso-latin-1...
//20
{"sigma",'x',"s",QString("σ"),963},
{"varsigma",'x',"V",QString("ς"),962},
{"tau",'x',"t",QString("τ"),964},
{"upsilon",'x',"u",QString("υ"),965},
{"phi",'x',"f",QString("ϕ"),981},
{"varphi",'x',"j",QString("φ"),966},
{"chi",'x',"c",QString("χ"),967},
{"psi",'x',"y",QString("ψ"),968},
{"omega",'x',"w",QString("ω"),969},
{"Gamma",'x',"G",QString("Γ"),915},
//30
{"Delta",'x',"D",QString("Δ"),916},
{"Theta",'x',"Q",QString("Θ"),920},
{"Lambda",'x',"L",QString("Λ"),923},
{"Xi",'x',"X",QString("Ξ"),926},
{"Pi",'x',"P",QString("Π"),928},
{"Sigma",'x',"S",QString("Σ"),931},
{"Upsilon",'x',"\\c!\\C",QString("Υ"),933},
{"Phi",'x',"F",QString("Φ"),934},
{"Psi",'x',"Y",QString("Ψ"),936},
{"Omega",'x',"W",QString("Ω"),937},
//40
{"sum",'x',"\\ce\\C",QString("Σ"),931},
{"prod",'x',"\\cU\\C",QString("Π"),928},
{"int",'x',"\\cr\\C",QString("ʃ"),643},
{"in",'x',"\\cN\\C",QString("ϵ"),1013},
{"forall",'x',"\"",QString("∀"),8704},
{"exists",'x',"$",QString("∃"),8707},
{"surd",'x',"\\cV\\C",QString("√"),8730},
{"Rightarrow",'x',"\\c^\\C",QString("⇒"),8658},
{"Leftarrow",'x',"\\c\\\\C",QString("⇐"),8656},
{"propto",'x',"\\c5\\C",QString("∝"),8733},
//50
{"partial",'x',"\\c6\\C",QString("∂"),8706},
{"infty",'x',"\\c%\\C",QString("∞"),8734},
{"cdot",'x',"\\cW\\C",QString("⋅"),8901},
{"times",'x',"\\c4\\C",QString("×"),215},
{"pm",'x',"\\c1\\C",QString("±"),177},
{"equiv",'x',"\\c:\\C",QString("≡"),8801},
{"approx",'x',"\\c;\\C",QString("≈"),8776},
{"ast",'x',"*",QString("∗"),8727},
{"bullet",'x',"\\c7\\C",QString("•"),8226},
{"div",'x',"\\c8\\C",QString("÷"),247},
//60
{"nabla",'x',"\\cQ\\C",QString("∂"),8706},
{"bot",'x',"^",QString("⊥"),8869},
{"clubsuit",'x',"\\c'\\C",QString("♣"),9827},
{"diamondsuit",'x',"\\c(\\C",QString("♦"),9830},
{"heartsuit",'x',"\\c)\\C",QString("♥"),9829},
{"spadesuit",'x',"\\c*\\C",QString("♠"),9824},
{"rightarrow",'x',"\\c.\\C",QString("→"),8594},
{"leftarrow",'x',"\\c,\\C",QString("←"),8592},
{"leq",'x',"\\c#\\C",QString("≤"),8804},
{"geq",'x',"\\c3\\C",QString("≥"),8805},
//70
{"angle",'x',"\\cP\\C",QString("∠"),8736},
{"Re",'x',"\\cB\\C",QString("ℜ"),8476},
{"Im",'x',"\\cA\\C",QString("ℑ"),8465},
{"neq",'x',"\\c9\\C",QString("≠"),8800},
{"sim",'x',"~",QString("~"),126},
{"hbar",'1',"h\\h{-0.55}\\v{-0.04}\\c/\\C\\h{0.3}\\v{0.04}",QString("ħ"),295},
{"oint",'x',"\\h{0.1}\\cr\\C\\f{Courier}\\h{-0.425}\\v{0.2}o\\v{-0.2}",QString("∮"),8750},
{"odot",'x',"\\h{0.2}\\v{0.05}\\cW\\C\\f{Courier}\\h{-0.435}\\v{0.055}o\\v{-0.105}",QString("⊙"),8857},
{"circ",'x',"o",QString("°"),176},
{"leftrightarrow",'x',"\\c+\\C",QString("↔"),8596},
//80
{"Leftrightarrow",'x',"\\c[\\C",QString("⇔"),8660},
{"AA",'0',"\\cE\\C",QString("Å"),197}
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

int replace_command(QString & command,char & new_font)
{
    int i;
    strcpy(dummy,command.toLatin1().constData());
    new_font=-1;//invalid
    //cout << "#" << command.toLatin1().constData() << "#" << endl;
    for (i=0;i<NUMBER_OF_LATEX_COMMANDS;i++)
    {
        if (!strcmp(dummy,allCommands[i].la_com))
        {
            strcpy(dummy,allCommands[i].ch);
            dummy[strlen(allCommands[i].ch)]='\0';
            new_font=allCommands[i].font;
            if (useQtFonts==true)
            {
                //command=allCommands[i].utf8;
                command=QString(QChar(allCommands[i].unicode));
            }
            break;
        }
    }
    if (useQtFonts==false)
    {
        if (new_font==-1)
        {
            return RETURN_FAILURE;
        }
        else
        {
            command=QString::fromLatin1(dummy);
            return RETURN_SUCCESS;
        }
    }
    else//QtFonts
    {
        if (i<NUMBER_OF_LATEX_COMMANDS)
            return RETURN_SUCCESS;
        else
            return RETURN_FAILURE;
    }
}

void remove_emptys(QString & text)//removes spaces at the beginning and the end of the string text
{
    if (text.isEmpty()) return;
    if (!text.at(0).isSpace() && text.at(text.length()-1).isSpace()) return;//no spaces at beginning or end
    int first_pos=0;//index of first character in text, that is not space
    int last_index=text.length()-1;//index of last character in text, that is not space
    while (first_pos<text.length() && text.at(first_pos).isSpace()) first_pos++;
    while (last_index>=0 && text.at(last_index).isSpace()) last_index--;
    if (first_pos>=text.length()) last_index=text.length()-1;
    if (last_index<0) last_index=0;
    text=text.mid(first_pos,last_index+1-first_pos);
    /*int pos=0;
char * dummy=new char[strlen(text)];
while (pos<strlen(text) && isspace(text[pos])) pos++;
strcpy(dummy,text+pos);
pos=strlen(dummy)-1;
while (pos>0 && isspace(dummy[pos])) pos--;
if (!isspace(dummy[pos])) pos++;
strcpy(text,dummy);//pos is now the position of the last character that is not ' '
text[pos]='\0';
delete[] dummy;*/
}

int remove_braces(QString & text)//removes {} at beginning and end if they belong together! returns the position of the closing brace to the initially opened one (returning 0 means: {} removed; returning -1 means: no {} found)
{
    static QChar opening('{'),closing('}');
    if (text.isEmpty()) return -1;
    int len=text.length();
    if (text.at(0)!=opening) return -1;//no braces at beginning of text
    int bracecounter=1;//first brace
    int i;
    for (i=1;i<len;i++)//do not look at first character since we know it is a brace
    {
        if (text.at(i)==opening) bracecounter++;
        if (text.at(i)==closing) bracecounter--;
        if (bracecounter==0) break;//the initially opend brace has been closed!
    }
    if (i<len-1)//we found a closing brace earlier than the the last character (this means contrary to expectation --> usually the case for strings like "{arg1}{arg2}" --> importatnt for splitter)
    {
        return i;
    }
    else if (i==len-1)
    {
        /*char * dummy=new char[len];
strcpy(dummy,text+1);
strcpy(text,dummy);
text[len-2]='\0';
delete[] dummy;*/
        text=text.mid(1,len-2);
        return 0;
    }
    else
        return -1;
}
/// HIER GEHT ES WEITER!
void arg_splitter(QString & argument,QString & argument1,QString & argument2)
{
    argument1.clear();
    argument2.clear();
    remove_emptys(argument);
    int ret=remove_braces(argument);
    while (ret==0)//continue removing unused braces
        ret=remove_braces(argument);
    if (ret==-1) return;//no two arguments
    //two arguments
    /// strcpy(argument1,argument+1);//+1 because we do not need the '{'
    /// argument1[ret-1]='\0';
    argument1=argument.mid(1,ret-1);
    //cout << "argument1=" << argument1.toLatin1().constData() << endl;
    /// strcpy(argument2,argument+ret+1);
    argument2=argument.mid(ret+1);
    //cout << "argument2=" << argument2.toLatin1().constData() << endl;
    ret=remove_braces(argument1);
    while (ret==0)//continue removing unused braces
        ret=remove_braces(argument1);
    ret=remove_braces(argument2);
    while (ret==0)//continue removing unused braces
        ret=remove_braces(argument2);
}

void find_first_command(QString & text,int & start_command,int & command_length,int * brace_pos)
{
    int len,i,bracecounter;
    len=text.length();
    bracecounter=0;
    start_command=-1;
    command_length=0;
    brace_pos[0]=brace_pos[1]=brace_pos[2]=brace_pos[3]=-1;
    for (i=0;i<len;i++)
    {
        if (text[i]==QChar('\\') || text[i]==QChar('_') || text[i]==QChar('^'))//beginning of a command
        {
            start_command=i;
            break;
        }
    }
    if (start_command!=-1)
    {
        if (text[start_command]==QChar('\\'))//we found a real command
        {
            for (i=start_command+1;i<=len;i++)
            {
                if (text[i]==QChar('\\') || text[i]==QChar(' ') || text[i]==QChar('=') || text[i]==QChar(')') || text[i]==QChar('(') || text[i]==QChar('{') || text[i]==QChar('_') || text[i]==QChar('^') || text[i]==QChar('\0'))
                {
                    command_length=i;//at the moment: this is the position of the first character after the command, not the length
                    break;
                }
            }
            if (text[command_length]==QChar('{'))
            {//brace found
                brace_pos[0]=command_length;
                bracecounter=1;
                for (i=brace_pos[0]+1;i<len;i++)
                {
                    if (text[i]==QChar('{')) bracecounter++;
                    if (text[i]==QChar('}')) bracecounter--;
                    if (text[i]==QChar('}') && bracecounter==0)
                    {
                        brace_pos[1]=i;
                        break;
                    }
                }//hopefully: closing brace found
                if (brace_pos[1]<len-1)//not the last character
                {
                    if (text[brace_pos[1]+1]==QChar('{'))//another brace found
                    {
                        brace_pos[2]=brace_pos[1]+1;
                        bracecounter=1;
                        for (i=brace_pos[2]+1;i<len;i++)
                        {
                            if (text[i]==QChar('{')) bracecounter++;
                            if (text[i]==QChar('}')) bracecounter--;
                            if (text[i]==QChar('}') && bracecounter==0)
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
            if (text[brace_pos[0]]!=QChar('\\') && text[brace_pos[0]]!=QChar('{'))//regular text --> only one character counts
            {
                brace_pos[1]=brace_pos[0]+1;
            }
            else
            {
                for (i=start_command+1;i<=len;i++)
                {
                    if (text[i]==QChar('\0'))
                    {
                        brace_pos[1]=i;
                        break;
                    }
                    if (text[i]==QChar('{')) bracecounter++;
                    if (text[i]==QChar('}')) bracecounter--;
                    if (bracecounter==0 && i<len-1 && text[i]==QChar('}') && text[i+1]==QChar('{'))
                    {
                        bracecounter++;
                        i++;//we already counted the next one
                        continue;
                    }
                    if ((text[i]==QChar('}') || text[i]==QChar(' ')) && bracecounter==0)
                    {
                        brace_pos[1]=i;
                        break;
                    }
                }
            }
        }
    }//end command found
}

void splitter(QString & text,int start_command,int command_length,int * brace_pos,QString & left_part,QString & middle_part,QString & argument_part,QString & right_part)
{
    static int first_brace,last_brace;
    first_brace=last_brace=brace_pos[0];
    if (first_brace!=-1)
        last_brace=brace_pos[1];
    if (brace_pos[3]!=-1)
        last_brace=brace_pos[3];
    /// strcpy(left_part,text);
    left_part=text;
    /// argument_part[0]=middle_part[0]=right_part[0]='\0';
    argument_part.clear();
    middle_part.clear();
    right_part.clear();
    if (start_command!=-1)
    {
        /// left_part[start_command]='\0';
        left_part=left_part.left(start_command);
        if (text.at(start_command)==QChar('\\'))
            middle_part=text.mid(start_command+1);
        /// strcpy(middle_part,text+start_command+1);
        else
            middle_part=text.mid(start_command);
        /// strcpy(middle_part,text+start_command);
        middle_part=middle_part.left(command_length);
        /// middle_part[command_length]='\0';
        if (first_brace!=-1)
        {
            argument_part=text.mid(first_brace);
            /// strcpy(argument_part,text+first_brace);
            if (text[first_brace]=='{')
            {
                argument_part=argument_part.left(last_brace-first_brace+1);
                /// argument_part[last_brace-first_brace+1]='\0';
                right_part=text.mid(last_brace+1);
                /// strcpy(right_part,text+last_brace+1);
            }
            else
            {
                argument_part=argument_part.left(last_brace-first_brace);
                /// argument_part[last_brace-first_brace]='\0';
                right_part=text.mid(last_brace);
                /// strcpy(right_part,text+last_brace);
            }
        }
        else
            right_part=text.mid(start_command+1+command_length);
        /// strcpy(right_part,text+start_command+1+command_length);
    }
}

int find_spec_function(QString & command)
{
    static int ret;
    ret=-1;
    strcpy(dummy,command.toLatin1().constData());
    for (int i=0;i<NUMBER_OF_SPECIAL_LATEX_COMMANDS;i++)
    {
        if (!strcmp(specCommands[i].la_com,dummy))
        {
            ret=i;
            break;
        }
    }
    return ret;
}

int recursive_replacer(QString & text)
{//takes text and searches for the first latex-command beginning with '\' or '^' or '_' and replaces this command
    //if '{' is encountered, the contents of the '{}' are given to another instance of this function
    //if no command is found, the text is returned unchanged, the same is the case if the command can not be identified
    //if no '{' is found the function continues to replace the following commands or copies the following text
    //returns the number of actual commands found
    int command_count=0;
    int len=text.length();
    QString errmessage;

    if (len==0) return 0;

    char new_font;
    ///char * result=new char[256+len];
    ///char * temp=new char[256+len];
    //cout << "input=#" << text << "#" << endl;
    /*char * left_part=new char[128+len];
char * middle_part=new char[128+len];
char * argument_part=new char[128+len];
char * argument1=new char[128+len];
char * argument2=new char[128+len];
char * right_part=new char[128+len];*/

    QString result,temp,temp2;
    QString left_part,middle_part,argument_part,argument1,argument2,right_part;

    int start_command=0,command_length,brace_pos[4];
    int res_pos=0,com_nr;

    remove_emptys(text);
    //cout << "after remove input=#" << text << "#" << endl;
    if (text.length()>0)
    {
        /// strcpy(temp,text);
        temp=text;
        while (start_command!=-1 && temp.length())
        {
            find_first_command(temp,start_command,command_length,brace_pos);
            splitter(temp,start_command,command_length,brace_pos,left_part,middle_part,argument_part,right_part);
            //cout << "pos=" << start_command << " len=" << command_length << " " << middle_part << endl;
            result=result.left(res_pos)+left_part;
            /// strcpy(result+res_pos,left_part);//copy the part that does not contain any useful commands
            res_pos+=left_part.length();
            command_count+=left_part.length();//we have to count normal characters as commands --> necessary to count length of commands for complicated commands
            arg_splitter(argument_part,argument1,argument2);//split this to be on the save side!
            //cout << "#" << left_part << "#" << middle_part << "#" << argument_part << "#" << right_part << "#" << endl;
            if (middle_part.length()>0)
            {
                if (replace_command(middle_part,new_font)==RETURN_SUCCESS)
                {
                    //cout << "new_font=" << new_font << endl;
                    command_count++;
                    if (new_font!=-1 && useQtFonts==false)
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
                    result=result.left(res_pos)+middle_part;
                    /// strcpy(result+res_pos,middle_part);
                    res_pos+=middle_part.length();
                    if (new_font!=-1 && useQtFonts==false)
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
                        result=result.left(res_pos)+QString(specCommands[com_nr].gr_com);
                        /// strcpy(result+res_pos,specCommands[com_nr].gr_com);
                        res_pos+=strlen(specCommands[com_nr].gr_com);
                        if (specCommands[com_nr].nr_of_exp_args==1)
                        {
                            command_count+=recursive_replacer(argument_part);
                            result=result.left(res_pos)+argument_part;
                            /// strcpy(result+res_pos,argument_part);
                            res_pos+=argument_part.length();
                        }
                        else
                        {
                            if (com_nr==5 && argument1.at(0)!=QChar('\0'))//special case 'frac'
                            {
                                int count1,count2;//,max_count;
                                count1=count2=0;
                                count1=recursive_replacer(argument1);
                                count2=recursive_replacer(argument2);
                                //max_count = count1>count2 ? count1 : count2;
                                temp2.clear();
                                //cout << "frac found " << count1 << " " << count2 << " max=" << max_count << endl;
                                if (count1>=count2)
                                {
                                    strcpy(dummy,"\\u");
                                }
                                else
                                {//lower part is longer
                                    sprintf(dummy,"\\h{%f}",(count2-count1)*0.25);
                                }
                                result=result.left(res_pos)+QString(dummy);
                                /// strcpy(result+res_pos,dummy);//commands to go on top of the line
                                res_pos+=strlen(dummy);
                                result=result.left(res_pos)+argument1;
                                /// strcpy(result+res_pos,argument1);//copy upper part = first argument of frac
                                res_pos+=argument1.length();
                                if (count1>=count2)
                                {
                                    sprintf(dummy,"\\U\\v{-1}\\h{%f}",-count1*0.5+0.25*(count1-count2));//switch from above to below line
                                }
                                else
                                {
                                    sprintf(dummy,"\\v{-1}\\h{%f}\\o",0.25*(count1-count2)-0.5*count1);
                                }
                                result=result.left(res_pos)+QString(dummy);
                                /// strcpy(result+res_pos,dummy);//go below line
                                res_pos+=strlen(dummy);
                                result=result.left(res_pos)+argument2;
                                /// strcpy(result+res_pos,argument2);//copy lower part = second argument of frac
                                res_pos+=argument2.length();
                                if (count1>=count2)
                                {
                                    sprintf(dummy,"\\v{0.5}\\h{%f}",(count1-count2)*0.25);
                                }
                                else
                                {
                                    sprintf(dummy,"\\O\\v{0.5}");
                                }
                                result=result.left(res_pos)+QString(dummy);
                                /// strcpy(result+res_pos,dummy);//end command
                                res_pos+=strlen(dummy);
                            }
                            else
                            {
                                errmessage=QObject::tr("command too complicated: #") + QString(middle_part) + QObject::tr("#\n") + QObject::tr("argument_part=#") + QString(argument_part) + QObject::tr("#argument1=#") + QString(argument1) + QObject::tr("#argument2=#") + QString(argument2);
                                errmsg(errmessage.toLocal8Bit().constData());
                                //cout << "command too complicated: #" << middle_part << "#" << endl;
                                //cout << "argument_part=#" << argument_part << "#argument1=#" << argument1 << "#argument2=#" << argument2 << endl;
                            }
                        }
                        result=result.left(res_pos)+QString(specCommands[com_nr].stop_com);
                        /// strcpy(result+res_pos,specCommands[com_nr].stop_com);
                        res_pos+=strlen(specCommands[com_nr].stop_com);
                    }
                    else
                    {
                        errmessage=QObject::tr("special command not found: #") + QString(middle_part) + QObject::tr("#");
                        //cout << "special command not found: #" << middle_part << "#" << endl;
                        errmsg(errmessage.toLocal8Bit().constData());
                        result=result.left(res_pos)+middle_part;
                        /// strcpy(result+res_pos,middle_part);//just copy the command...
                        res_pos+=middle_part.length();
                        command_count+=middle_part.length();
                    }
                }
            }
            command_count+=recursive_replacer(right_part);//right part is clear with this
            result=result.left(res_pos)+right_part;
            /// strcpy(result+res_pos,right_part);//go on with right part
            res_pos+=right_part.length();
            //cout << "temp=#" << temp << "#" << endl;
            /// temp[0]='\0';
            temp.clear();
        }//end of while-loop
        //cout << "result=#" << result << "#" << endl;
        /// strcpy(text,result);
        text=result;
    }//end test for empty 'text'

    /*
delete[] left_part;
delete[] middle_part;
delete[] argument_part;
delete[] argument1;
delete[] argument2;
delete[] right_part;
delete[] result;
delete[] temp;
*/

    return command_count;
}

void last_settings_in_string(QString text,int & font,double & oblique,double & vshift,double & hshift,double & zoom,bool & underline,bool & overline)
{
    static QRegularExpressionMatch rm;
    static QRegularExpression regex_f1("\\\\f\\{");//should find \f{
    static QRegularExpression regex_f2("\\\\\\d");//should find \0...\9
    static QRegularExpression regex_obl1("\\\\q");
    static QRegularExpression regex_obl2("\\\\Q");
    static QRegularExpression regex_obl3("\\\\l\\{");
    static QRegularExpression regex_vshift1("\\\\v\\{");
    static QRegularExpression regex_hshift1("\\\\h\\{");
    static QRegularExpression regex_zoom1("\\\\\\+");
    static QRegularExpression regex_zoom2("\\\\\\-");
    static QRegularExpression regex_zoom3("\\\\z\\{");
    static QRegularExpression regex_ul1("\\\\u");
    static QRegularExpression regex_ul2("\\\\U");
    static QRegularExpression regex_ol1("\\\\o");
    static QRegularExpression regex_ol2("\\\\O");

    bool ok;
    static QString result,mtext;
    static char mtext2[32];
    static int pos1,pos2,pos3;
    static double tmp_val;

    result=text;
    ///font=-1;//original font --> \f{}

    //First part: find font type
    pos1=pos2=pos3=0;
    rm=regex_f1.match(result,pos1);
    pos1=rm.capturedStart();
    rm=regex_f1.match(result,pos2);
    pos2=rm.capturedStart();
    while (pos1>=0 || pos2>=0)
    {
        if (pos1>=0)//long font-nr like \f{Symbol}
        {
            pos2=result.indexOf(QString("}"),pos1+1);
            mtext=result.mid(pos1+3,pos2-pos1-3);
            //cout << "font mtext=#" << mtext.toLocal8Bit().constData() << "#" << endl;
            if (mtext.length()<=0)
                font=-1;
            else
            {
                strcpy(mtext2,mtext.toLocal8Bit());
                font=get_font_by_name(mtext2);
            }
            pos1=pos2=pos2+1;
        }
        else//short font-nr like \5
        {
            /// mtext2[0]=result.at(pos2+1).toLatin1();
            /// mtext2[1]='\0';
            /// font=atoi(mtext2);
            font=result.mid(pos2+1).toInt(&ok);
            //cout << "ueber number: mfont=" << font << endl;
            pos1=pos2=pos2+2;
        }
        rm=regex_f1.match(result,pos1);
        pos1=rm.capturedStart();
        rm=regex_f1.match(result,pos2);
        pos2=rm.capturedStart();
    }
    //now font should be known

    //Second part: find oblique setting
    pos1=pos2=pos3=0;
    rm=regex_obl1.match(result,pos1);
    pos1=rm.capturedStart();
    rm=regex_obl2.match(result,pos2);
    pos2=rm.capturedStart();
    rm=regex_obl3.match(result,pos3);
    pos3=rm.capturedStart();
    while (pos1>=0 || pos2>=0 || pos3>=0)
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
            //cout << "oblique mtext=#" << mtext.toLocal8Bit().constData() << "#" << endl;
            if (mtext.length()>0)
            {
                /// strcpy(mtext2,mtext.toLocal8Bit());
                /// oblique+=atof(mtext2);
                tmp_val=mtext.toDouble(&ok);
                if (ok==true)
                    oblique+=tmp_val;
                else
                    cout << "Error converting " << mtext.toLocal8Bit().constData() << " to Double." << endl;
            }
            pos1=pos2=pos3=pos2+1;
        }
        rm=regex_obl1.match(result,pos1);
        pos1=rm.capturedStart();
        rm=regex_obl2.match(result,pos2);
        pos2=rm.capturedStart();
        rm=regex_obl3.match(result,pos3);
        pos3=rm.capturedStart();
    }
    //now we should know the oblique setting

    //Third part: find shifts
    pos1=pos2=pos3=0;
    rm=regex_vshift1.match(result,pos1);
    pos1=rm.capturedStart();
    rm=regex_hshift1.match(result,pos2);
    pos2=rm.capturedStart();
    while (pos1>=0 || pos2>=0)
    {
        if (pos1>=0)//vshift
        {
            pos2=result.indexOf(QString("}"),pos1+1);
            mtext=result.mid(pos1+3,pos2-pos1-3);
            //cout << "vshift mtext=#" << mtext.toLocal8Bit().constData() << "#" << endl;
            if (mtext.length()>0)
            {
                /// strcpy(mtext2,mtext.toLocal8Bit());
                /// vshift+=atof(mtext2);
                tmp_val=mtext.toDouble(&ok);
                if (ok==true)
                    vshift+=tmp_val;
                else
                    cout << "Error converting " << mtext.toLocal8Bit().constData() << " to Double." << endl;
            }
            pos1=pos2=pos3=pos2+1;
        }
        else//hshift
        {
            pos3=result.indexOf(QString("}"),pos2+1);
            mtext=result.mid(pos2+3,pos3-pos2-3);
            //cout << "hshift mtext=#" << mtext.toLocal8Bit().constData() << "#" << endl;
            if (mtext.length()>0)
            {
                /// strcpy(mtext2,mtext.toLocal8Bit());
                /// hshift+=atof(mtext2);
                tmp_val+=mtext.toDouble(&ok);
                if (ok==true)
                    hshift+=tmp_val;
                else
                    cout << "Error converting " << mtext.toLocal8Bit().constData() << " to Double." << endl;
            }
            pos1=pos2=pos3=pos3+1;
        }
        rm=regex_vshift1.match(result,pos1);
        pos1=rm.capturedStart();
        rm=regex_hshift1.match(result,pos2);
        pos2=rm.capturedStart();
    }
    //now we should know the actual shifts

    //Fourth part: find zoom factor
    pos1=pos2=pos3=0;
    rm=regex_zoom1.match(result,pos1);
    pos1=rm.capturedStart();
    rm=regex_zoom2.match(result,pos2);
    pos2=rm.capturedStart();
    rm=regex_zoom3.match(result,pos3);
    pos3=rm.capturedStart();
    while (pos1>=0 || pos2>=0 || pos3>=0)
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
            //cout << "zoom mtext=#" << mtext.toLocal8Bit().constData() << "#" << endl;
            if (mtext.length()>0)
            {
                /// strcpy(mtext2,mtext.toLocal8Bit());
                /// zoom*=atof(mtext2);
                tmp_val+=mtext.toDouble(&ok);
                if (ok==true)
                    zoom*=tmp_val;
                else
                    cout << "Error converting " << mtext.toLocal8Bit().constData() << " to Double." << endl;
            }
            else
                zoom=1.0;
            pos1=pos2=pos3=pos2+1;
        }
        rm=regex_zoom1.match(result,pos1);
        pos1=rm.capturedStart();
        rm=regex_zoom2.match(result,pos2);
        pos2=rm.capturedStart();
        rm=regex_zoom3.match(result,pos3);
        pos3=rm.capturedStart();
    }
    //now we should know the zoom factor

    //Fifth part: find underline
    pos1=pos2=pos3=0;
    rm=regex_ul1.match(result,pos1);
    pos1=rm.capturedStart();
    rm=regex_ul2.match(result,pos2);
    pos2=rm.capturedStart();
    while (pos1>=0 || pos2>=0)
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
        rm=regex_ul1.match(result,pos1);
        pos1=rm.capturedStart();
        rm=regex_ul2.match(result,pos2);
        pos2=rm.capturedStart();
    }
    //now we should know wether underline is present

    //Sixth part: find overline
    pos1=pos2=pos3=0;
    rm=regex_ol1.match(result,pos1);
    pos1=rm.capturedStart();
    rm=regex_ol2.match(result,pos2);
    pos2=rm.capturedStart();
    while (pos1>=0 || pos2>=0)
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
        rm=regex_ol1.match(result,pos1);
        pos1=rm.capturedStart();
        rm=regex_ol2.match(result,pos2);
        pos2=rm.capturedStart();
    }
//now we should know whether overline is present
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
    static int pos,pos2,altpos;//,ret;
    static QString str,str2;
    static QString ref("$$");
    static QString result,intermediate,new_text;
    static QString text_before,text_incl_options;
    ///cout << "to Convert #" << text.toLocal8Bit().constData() << "#-->#";
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
        //cout << "str=" << str.toLocal8Bit().constData() << " ZZZZ=" << zoom << endl;
        if (pos==altpos) break;
        //cout << "pos=" << pos << " pos2=" << pos2 << endl;
        altpos=pos;
        //now pos=$$ and pos2=first normal text position (maybe the beginning or the first text after the last closing '$$')
        text_before=str.mid(pos2,pos-pos2);
        last_settings_in_string(text_before,font_nr,oblique,vshift,hshift,zoom,underline,overline);
        include_spec_text_settings(text_incl_options,font_nr,oblique,vshift,hshift,zoom,underline,overline,true);
        /// text_incl_options contains - apart from the font - only special text-commands like underline, overline or oblique
        //cout << "text before #" << text_before.toLocal8Bit().constData() << "#" << " ZZZZ=" << zoom<< endl;
        //cout << "text incl. options #" << text_incl_options.toLocal8Bit().constData() << "#" << endl;
        pos+=2;//beginning of commands after '$$'
        pos2=str.indexOf(ref,pos);//search for end of command
        str2=str.mid(pos,pos2-pos);
        intermediate=ref+str2+ref;//The complete LaTeX-command framed by '$$'
        //cout << "intermediate #" << intermediate.toLocal8Bit().constData() << "#" << endl;
        /// strcpy(dummy,str2.toLocal8Bit());//dummy is now the LaTeX-command without the '$$'
        //cout << "without dollar #" << dummy << "#" << endl;
        //ret=recursive_replacer(str2);
        (void)recursive_replacer(str2);//everything will be replaced by Grace-commands here
        new_text=str2+text_incl_options;
        //result.replace(intermediate,QString(dummy)+text_incl_options);//replace commands in actual string
        result.replace(pos-2,intermediate.length(),new_text);//replace only one command
        pos-=2;//beginning first '$$' in original text = first position that has been replaced
        //pos+=strlen(dummy);//this should be the first character after the replacement --> continue search here
        pos+=new_text.length();
        //pos+=text_incl_options.length();
        str=result;
        //cout << "str=#" << str.toLocal8Bit().constData() << "# result=#" << result.toLocal8Bit().constData() << "#" << " ZZZZ=" << zoom<< endl;
    }
    ///cout << result.toLocal8Bit().constData() << "#" << endl;
    text=result;
}

