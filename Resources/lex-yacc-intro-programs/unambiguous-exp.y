%term id

%%

E : E '+' T
  | T
  ;

T : T '*' F
  | F
  ;

F : id
