/* $b01 */
#include "code-gen.h"
using namespace std;
int tCG::p01(){ // S -> PROG
  string header ="/*  " + lex.Authentication()+"   */\n";
  header += "#include \"mlisp.h\"\n";
  header += declarations;
  header += "//________________ \n";
    S1->obj = header + S1->obj;
	return 0;}
int tCG::p02(){ //   PROG -> CALCS
    S1->obj = "int main(){\n" + S1->obj
    + " std::cin.get();\n return 0;\n}\n";
	return 0;}
int tCG::p03(){ //   PROG -> DEFS
    S1->obj += "int main(){\n"
    " display(\"No calculations!\");newline();\n"
    " std::cin.get();\n return 0;\n}\n";
	return 0;}
int tCG::p04(){ //   PROG -> DEFS CALCS
    S1->obj += "int main(){\n" + S2->obj
    + " std::cin.get();\n return 0;\n}\n";
	return 0;}
int tCG::p05(){ //  CALCS -> CALC
	return 0;}
int tCG::p06(){ //  CALCS -> CALCS CALC
 S1->obj += S2->obj;
	return 0;}
int tCG::p07(){ //   CALC -> E
 S1->obj = " display("+S1->obj+"); newline();\n";
	return 0;}
int tCG::p08(){ //   CALC -> BOOL
	return 0;}
int tCG::p09(){ //   CALC -> STR
	return 0;}
int tCG::p10(){ //   CALC -> DISPSET
	return 0;}
int tCG::p11(){ //      E -> $id
 S1->obj = decor(S1->name);
	return 0;}
int tCG::p12(){ //      E -> CONST
	return 0;}
int tCG::p13(){ //      E -> ADD
	return 0;}
int tCG::p14(){ //      E -> SUB
	return 0;}
int tCG::p15(){ //      E -> DIV
	return 0;}
int tCG::p16(){ //      E -> MUL
	return 0;}
int tCG::p17(){ //      E -> COND
	return 0;}
int tCG::p18(){ //      E -> CPROC
	return 0;}
int tCG::p19(){ //  CONST -> $zero
    S1->obj = decor(S1->name);
	return 0;}
int tCG::p20(){ //  CONST -> $dec
    S1->obj = decor(S1->name);
	return 0;}
int tCG::p21(){ //    SUB -> ( - E E )
    S1->obj = "(" + S3->obj + " - " + S4->obj + ")";
	return 0;}
int tCG::p22(){ //    DIV -> ( / E E )
    S1->obj = "(" + S3->obj + " / " + S4->obj + ")";
	return 0;}
int tCG::p23(){ //    ADD -> HADD E )
    if(S1->count == 0) {
        S1->obj += S2->obj + ")";
    }
    else {
        S1->obj += " + " + S2->obj + ")";
    }
	return 0;}
int tCG::p24(){ //   HADD -> ( +
    S1->obj = "(";
	return 0;}
int tCG::p25(){ //   HADD -> HADD E
    S1->obj += S2->obj;
    ++S1->count;
	return 0;}
int tCG::p26(){ //    MUL -> HMUL E )
    if(S1->count == 0) {
        S1->obj += S2->obj + ")";
    }
    else {
        S1->obj += " * " + S2->obj + ")";
    }
	return 0;}
int tCG::p27(){ //   HMUL -> ( *
    S1->obj = "(";
	return 0;}
int tCG::p28(){ //   HMUL -> HMUL E
    S1->obj += S2->obj;
    ++S1->count;
	return 0;}
int tCG::p29(){ //   COND -> HCOND ELSE )
    S1->obj += S2->obj;
	return 0;}
int tCG::p30(){ //   COND -> HCOND CLAUS )
    S1->obj += S2->obj + "_infinity";
	return 0;}
int tCG::p31(){ //  HCOND -> ( cond
	return 0;}
int tCG::p32(){ //  HCOND -> HCOND CLAUS
    S1->obj += S2->obj;
	return 0;}
int tCG::p33(){ //  CLAUS -> HCLAUS E )
    S1->obj += " ? " + S2->obj + " : ";
	return 0;}
int tCG::p34(){ // HCLAUS -> ( BOOL
    S1->obj += S2->obj;
	return 0;}
int tCG::p35(){ // HCLAUS -> HCLAUS INTER
    S1->obj += S2->obj;
	return 0;}
int tCG::p36(){ //   ELSE -> HELSE E )
    S1->obj += S2->obj;
	return 0;}
int tCG::p37(){ //  HELSE -> ( else
    S1->obj = " : ";
	return 0;}
int tCG::p38(){ //  HELSE -> HELSE INTER
	return 0;}
int tCG::p39(){ //  CPROC -> HCPROC )
    S1->obj += ")";
	return 0;}
int tCG::p40(){ // HCPROC -> ( $id
    S1->obj = decor(S2->name) + "(";
    S1->count = 0;
	return 0;}
int tCG::p41(){ // HCPROC -> HCPROC E
    if(S1->count == 0) {
        S1->obj += S2->obj;
        ++S1->count;
    }
    else {
        S1->obj += ", " + S2->obj;
    }
	return 0;}
int tCG::p42(){ //   BOOL -> $bool
    S1->obj = (S1->name == "#t" ? "true" : "false");
	return 0;}
int tCG::p43(){ //   BOOL -> $idq
    S1->obj = decor(S1->name);
	return 0;}
int tCG::p44(){ //   BOOL -> CPRED
	return 0;}
int tCG::p45(){ //   BOOL -> REL
	return 0;}
int tCG::p46(){ //   BOOL -> AND
	return 0;}
int tCG::p47(){ //   BOOL -> ( not BOOL )
    S1->obj = "( !" + S3->obj + ")";
	return 0;}
int tCG::p48(){ //  CPRED -> HCPRED )
	return 0;}
int tCG::p49(){ // HCPRED -> ( $idq
	return 0;}
int tCG::p50(){ // HCPRED -> HCPRED ARG
	return 0;}
int tCG::p51(){ //    ARG -> E
	return 0;}
int tCG::p52(){ //    ARG -> BOOL
	return 0;}
int tCG::p53(){ //    REL -> ( > E E )
    S1->obj = "(" + S3->obj + " > " + S4->obj + ")";
	return 0;}
int tCG::p54(){ //    REL -> ( = E E )
    S1->obj = "(" + S3->obj + " == " + S4->obj + ")";
	return 0;}
int tCG::p55(){ //    AND -> HAND BOOL )
    S1->obj += S2->obj + ")";
	return 0;}
int tCG::p56(){ //   HAND -> ( and
    S1->obj = "(";
	return 0;}
int tCG::p57(){ //   HAND -> HAND BOOL
    S1->obj += S2->obj + " && ";
	return 0;}
int tCG::p58(){ //    STR -> $str
    S1->obj = decor(S1->name);
	return 0;}
int tCG::p59(){ //    STR -> SIF
	return 0;}
int tCG::p60(){ //    SIF -> ( if BOOL STR STR )
    S1->obj = "( " + S3->obj + " ? " + S4->obj + " : " + S5->obj + ")";
	return 0;}
int tCG::p61(){ //    SET -> ( set! $id E )
    S1->obj += decor(S3->name) + " = " + S4->obj;
	return 0;}
int tCG::p62(){ //DISPSET -> ( display E )
    S1->obj = "display(" + S3->obj + ")";
	return 0;}
int tCG::p63(){ //DISPSET -> ( display BOOL )
    S1->obj = "display(" + S3->obj + ")";
	return 0;}
int tCG::p64(){ //DISPSET -> ( display STR )
    S1->obj = "display(" + S3->obj + ")";
	return 0;}
int tCG::p65(){ //DISPSET -> ( newline )
    S1->obj = "newline()";
	return 0;}
int tCG::p66(){ //DISPSET -> SET
	return 0;}
int tCG::p67(){ //  INTER -> DISPSET
	return 0;}
int tCG::p68(){ //  INTER -> E
	return 0;}
int tCG::p69(){ //   DEFS -> DEF
	return 0;}
int tCG::p70(){ //   DEFS -> DEFS DEF
    S1->obj += "\n" + S2->obj;
	return 0;}
int tCG::p71(){ //    DEF -> PRED
	return 0;}
int tCG::p72(){ //    DEF -> VAR
	return 0;}
int tCG::p73(){ //    DEF -> PROC
	return 0;}
int tCG::p74(){ //   PRED -> HPRED BOOL )
    S1->obj += "return " + S2->obj + ";\n}";
	return 0;}
int tCG::p75(){ //  HPRED -> PDPAR )
    S1->obj += ")";
    declarations += S1->obj + ";\n";
	return 0;}
int tCG::p76(){ //  PDPAR -> ( define ( $idq
    S1->obj = "bool " + decor(S4->name) + "(";
    S1->count = 0;
	return 0;}
int tCG::p77(){ //  PDPAR -> PDPAR $idq
    if(S1->count == 0) {
        S1->obj += S2->obj;
        ++S1->count;
    }
    else {
        S1->obj += ", " + S2->obj;
    }
	return 0;}
int tCG::p78(){ //  PDPAR -> PDPAR $id
    if(S1->count == 0) {
        S1->obj += S2->obj;
        ++S1->count;
    }
    else {
        S1->obj += ", " + S2->obj;
    }
	return 0;}
int tCG::p79(){ //    VAR -> ( define $id CONST )
    declarations += "double " + decor(S3->name) + " = " + S4->obj + ";\n";
	return 0;}
int tCG::p80(){ //   PROC -> HPROC LET )
    S1->obj += S2->obj + "\n}";
	return 0;}
int tCG::p81(){ //   PROC -> HPROC E )
    S1->obj += "return " + S2->obj + ";\n}\n";
	return 0;}
int tCG::p82(){ //  HPROC -> PCPAR )
    S1->obj += ")";
    declarations += S1->obj + ";\n";
    S1->obj += " {\n";
	return 0;}
int tCG::p83(){ //  HPROC -> HPROC INTER
    S1->obj += S2->obj + ";\n";
	return 0;}
int tCG::p84(){ //  PCPAR -> ( define ( $id
    S1->obj += "double " + decor(S4->name) + "(";
    S1->count = 0;
	return 0;}
int tCG::p85(){ //  PCPAR -> PCPAR $id
    if( S1->count == 0 ) {
        S1->obj += "double " + decor(S2->name);
        ++S1->count; 
    }
    else {
        S1->obj += ", dobule " + decor(S2->name);
    }
	return 0;}
int tCG::p86(){ //    LET -> HLET E )
    S1->obj += S2->obj + "\n}";
	return 0;}
int tCG::p87(){ //   HLET -> LETLOC )
    S1->obj += "\n}";
	return 0;}
int tCG::p88(){ //   HLET -> HLET INTER
	return 0;}
int tCG::p89(){ // LETLOC -> ( let (
    S1->obj = "{\n";
	return 0;}
int tCG::p90(){ // LETLOC -> LETLOC LETVAR
    S1->obj += S2->obj;
	return 0;}
int tCG::p91(){ // LETVAR -> ( $id E )
    S1->obj = "double " + decor(S2->name) + " = " + S3->obj + ";\n";
	return 0;}
//_____________________
int tCG::p92(){return 0;} int tCG::p93(){return 0;} 
int tCG::p94(){return 0;} int tCG::p95(){return 0;} 
int tCG::p96(){return 0;} int tCG::p97(){return 0;} 
int tCG::p98(){return 0;} int tCG::p99(){return 0;} 
int tCG::p100(){return 0;} int tCG::p101(){return 0;} 
int tCG::p102(){return 0;} int tCG::p103(){return 0;} 
int tCG::p104(){return 0;} int tCG::p105(){return 0;} 
int tCG::p106(){return 0;} int tCG::p107(){return 0;} 
int tCG::p108(){return 0;} int tCG::p109(){return 0;} 
int tCG::p110(){return 0;} 

