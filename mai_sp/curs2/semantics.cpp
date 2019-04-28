/* $mlisp19 */
#include "semantics.h"
#include "semempty.cpp"
using namespace std;
void tSM::init(){
  globals.clear();
  locals.clear();
  params.clear();
  scope =0; // вне процедуры
//константы:
  globals["e"] = 
//          properties
    tgName(VAR|DEFINED|BUILT);
  globals["pi"] = 
    tgName(VAR|DEFINED|BUILT);
//
// элементарные процедуры:
  globals["remainder"] =
//          properties       arity
    tgName(PROC|DEFINED|BUILT, 2);
  globals["abs"] =
    tgName(PROC|DEFINED|BUILT, 1);
// ...
// только те процедуры, которые использованы
// в СВОИХ контрольных задачах
	return;}
int tSM::p01(){ //       S -> PROG
 bool error=false;
 for(tGlobal::iterator it=globals.begin();
     it!=globals.end();
     ++it){
// Просмотреть таблицу глобальных имен
// и в сообщении об ошибках указать имена
// ВСЕХ вызванных, но не определенных процедур,
// а также использованных, но не определенных
// глобальных переменных. Сообщения отметить [!].
// Кроме того, ПРЕДУПРЕДИТЬ обо ВСЕХ определенных,
// но не использованных процедурах и переменных,
// за исключением встроенных. Сообщения отметить [?].
//   it->first   имя
//   it->second  учетная запись
// ...
 }//for...
 if(error) return 1;
	return 0;}
int tSM::p11(){ //       E -> $id
	return 0;}
int tSM::p45(){ //   CPROC -> HCPROC )
 string name = S1->name;
 int    count= S1->count;
 if(scope>1){// внури тела let
    if(locals.count(name)){// локальное имя
//p45-1.ss
      ferror_message="[!]Procedure application:"
        " local variable '"+name+
        "' shadows the procedure!";
      return 1;
    } // if locals ...
  } // if scope ...
 if(scope>0){// внутри процедуры
    if(params.count(name)){// имя параметра
//p45-2.ss
      ferror_message="[!]Procedure application:"
        " parameter '"+name+
        "' shadows the procedure!";
      return 1;
    }// if params...
 }// if scope... 
 do{
//  найти имя в глобальной таблице
    tgName& ref = globals[name];
    if(ref.empty()){//неизвестное имя
//        создать новую учетную запись
      ref = tgName(PROC|USED, count);
      break;
     }

// имя найдено
    if(!ref.test(PROC)){//не процедура
//p45-3.ss
      ferror_message="[!]Procedure application:"
        "  '"+ name+
        "' is not a procedure!";
      return 1;
    }

    if(ref.arity!=count){//число аргументов
//                не равно числу параметров 
     std::ostringstream buf;
     buf<<"[!]Procedure application: '"<<name<< "' "
//p45-4.ss
        <<(ref.test(DEFINED) ? "expects " // процедура
//                                      уже определена
//p45-5.ss

// процедура еще не определена, но уже вызывалась ранее         
               : "has been called already\n\t with "
          )
        <<ref.arity<<" argument"
        <<(ref.arity!=1 ?"s":"")
        <<", given: "<<count<<" !";
     ferror_message = buf.str();
     return 1;
    }
// ошибок нет
    ref.set(USED);//имя использовано
   }while(false);
	return 0;}
int tSM::p46(){ //  HCPROC -> ( $id
 S1->name = S2->name;
 S1->count = 0;
	return 0;}
int tSM::p47(){ //  HCPROC -> HCPROC E
 ++S1->count;
	return 0;}
int tSM::p49(){ //    BOOL -> $idq
	return 0;}
int tSM::p55(){ //   CPRED -> HCPRED )
	return 0;}
int tSM::p56(){ //  HCPRED -> ( $idq
	return 0;}
int tSM::p57(){ //  HCPRED -> HCPRED ARG
	return 0;}
int tSM::p58(){ //     ARG -> E
	return 0;}
int tSM::p59(){ //     ARG -> BOOL
	return 0;}
int tSM::p74(){ //     SET -> ( set! $id E )
	return 0;}
int tSM::p87(){ //    PRED -> HPRED BOOL )
	return 0;}
int tSM::p88(){ //   HPRED -> PDPAR )
 scope = 1;
	return 0;}
int tSM::p89(){ //   PDPAR -> ( define ( $idq
	return 0;}
int tSM::p90(){ //   PDPAR -> PDPAR $idq
	return 0;}
int tSM::p91(){ //   PDPAR -> PDPAR $id
	return 0;}
int tSM::p92(){ //     VAR -> ( define $id CONST )
	return 0;}
int tSM::p93(){ //    PROC -> HPROC LET )
 params.clear();
 scope = 0;
	return 0;}
int tSM::p94(){ //    PROC -> HPROC E )
 params.clear();
 scope = 0;
	return 0;}
int tSM::p95(){ //   HPROC -> PCPAR )
 scope = 1;
	return 0;}
int tSM::p97(){ //   PCPAR -> ( define ( $id
 S1->name = S4->name;
 S1->count = 0;
	return 0;}
int tSM::p98(){ //   PCPAR -> PCPAR $id
    if(params.count(S2->name)){
//p98-1.ss
      ferror_message=
        "[!]Procedure definition: in '"
        +S1->name+
        "' duplicate parameter identifier '"
        +S2->name+"'!";
      return 1;
    }
    params.insert(S2->name);
 ++S1->count;
	return 0;}
int tSM::p99(){ //     LET -> HLET E )
 locals.clear();
	return 0;}
int tSM::p100(){ //    HLET -> LETLOC )
  scope=2;
	return 0;}
int tSM::p102(){ //  LETLOC -> ( let (
	return 0;}
int tSM::p104(){ //  LETVAR -> ( $id E )
	return 0;}
//_____________________
