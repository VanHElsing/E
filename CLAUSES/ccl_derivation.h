/*-----------------------------------------------------------------------

File  : ccl_derivations.h

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Datatypes and definitions for compact representation of derivations
  of a clause. 

  Copyright 2013 by the author.
  This code is released under the GNU General Public Licence.
  See the file COPYING in the main CLIB directory for details.
  Run "eprover -h" for contact information.

Changes

<1> Thu Feb 14 00:21:15 CET 2013
    New

-----------------------------------------------------------------------*/

#ifndef CCL_DERIVATION

#define CCL_DERIVATION

#include <ccl_inferencedoc.h>
#include <ccl_clauses.h>
#include <ccl_formula_wrapper.h>

/*---------------------------------------------------------------------*/
/*                    Data type declarations                           */
/*---------------------------------------------------------------------*/


typedef enum
{
   DONop,
   DOQuote,
   /* For simplifying inferences, the main premise is implicit */
   DORewrite,
   DOUnfold,
   DOApplyDef,
   DOContextSR,
   DODesEqRes, 
   DOSR,
   DOAcRes,
   DOCondense,
   DONormalize,
   DOEvalAnswers,
   /* Simplification/Modfication for FOF */
   DONegateConjecture,
   DOFofSimplify,
   DOFNNF,   
   DOShiftQuantors,
   DOVarRename,
   DOSkolemize,
   DODistDisjunctions,
   DOAnnoQuestion,
   /* Generating inferences */
   DOParamod,
   DOSimParamod,
   DOOrderedFactor,
   DOEqFactor,
   DOEqRes,
   /* CNF conversion and similar */
   DOSplitEquiv,
   DOIntroDef,
   DOSplitConjunct
}OpCodes;


typedef enum
{
   Arg1Fof = 1<<8,
   Arg1Cnf = 1<<9,
   Arg1Num = 1<<10,
   Arg2Fof = 1<<11,
   Arg2Cnf = 1<<12,
   Arg2Num = 1<<13,
}ArgDesc;


typedef enum
{
   DCNop              = DONop,
   DCCnfQuote         = DOQuote|Arg1Cnf,
   DCFofQuote         = DOQuote|Arg1Fof,
   /* For simplifying inferences, the main premise is implicit */
   DCRewrite          = DORewrite|Arg1Cnf,
   DCUnfold           = DOUnfold|Arg1Cnf,
   DCApplyDef         = DOApplyDef|Arg1Fof,
   DCContextSR        = DOContextSR|Arg1Cnf,
   DCSR               = DOSR|Arg1Cnf,
   DCDesEqRes         = DODesEqRes, /* Doubled because its simplifying here */
   DCACRes            = DOAcRes|Arg1Num,
   DCCondense         = DOCondense,
   DCNormalize        = DONormalize,
   DCEvalAnswers      = DOEvalAnswers,
   /* Simplification/Modfication for FOF */
   DCNegateConjecture = DONegateConjecture,
   DCFofSimplify      = DOFofSimplify,
   DCFNNF             = DOFNNF,   
   DCShiftQuantors    = DOShiftQuantors,
   DCVarRename        = DOVarRename,
   DCSkolemize        = DOSkolemize,
   DCDistDisjunctions = DODistDisjunctions,
   DCAnnoQuestion     = DOAnnoQuestion,
   /* Generating inferences */
   DCParamod          = DOParamod |Arg1Cnf|Arg2Cnf,
   DCSimParamod       = DOSimParamod|Arg1Cnf|Arg2Cnf,
   DCOrderedFactor    = DOOrderedFactor|Arg1Cnf,
   DCEqFactor         = DOEqFactor|Arg1Cnf,
   DCEqRes            = DOEqRes|Arg1Cnf,
   /* CNF conversion and similar */
   DCSplitEquiv       = DOSplitEquiv|Arg1Fof,
   DCIntroDef         = DOIntroDef,
   DCSplitConjunct    = DOSplitConjunct|Arg1Fof
}DerivationCodes;


typedef enum
{
   PONoObject = 0,
   POSimpleDeriviation = 1,
   PODetailedDerivation = 2,
   POSingleStepDerivation = 3
}ProofObjectType;


typedef struct derived_cell
{
   long       ref_count;
   bool       is_root;
   bool       is_fresh;
   Clause_p   clause;
   WFormula_p formula;
}DerivedCell, *Derived_p;


typedef struct derivation_cell
{
   bool       ordered;
   Sig_p      sig;
   PObjTree_p deriv;
   PStack_p   roots;
   PStack_p   ordered_deriv;
}DerivationCell, *Derivation_p;


/*---------------------------------------------------------------------*/
/*                Exported Functions and Variables                     */
/*---------------------------------------------------------------------*/


extern ProofObjectType BuildProofObject;

#define DCOpHasCnfArg1(op)   ((op)&Arg1Cnf)
#define DCOpHasFofArg1(op)   ((op)&Arg1Fof)
#define DCOpHasNumArg1(op)    ((op)&Arg1Num)
#define DCOpHasParentArg1(op) ((op)&(Arg1Cnf|Arg1Fof))
#define DCOpHasArg1(op)    ((op)&(Arg1Cnf|Arg1Fof|Arg1Num))

#define DCOpHasCnfArg2(op)    ((op)&Arg2Cnf)
#define DCOpHasFofArg2(op)    ((op)&Arg2Fof)
#define DCOpHasNumArg2(op)    ((op)&Arg2Num)
#define DCOpHasParentArg2(op) ((op)&(Arg2Cnf|Arg2Fof))
#define DCOpHasArg2(op)       ((op)&(Arg2Cnf|Arg2Fof|Arg2Num))

#define DPOpGetOpCode(op)  ((op)&127)

void ClausePushDerivation(Clause_p clause, DerivationCodes op, 
                          void* arg1, void* arg2);

void ClausePushACResDerivation(Clause_p clause, Sig_p sig);
void WFormulaPushDerivation(WFormula_p form, DerivationCodes op, 
                           void* arg1, void* arg2);

long DerivStackExtractParents(PStack_p derivation, 
                              Sig_p sig,
                              PStack_p res_clauses, 
                              PStack_p res_formulas);


#define DerivedCellAlloc() (DerivedCell*)SizeMalloc(sizeof(DerivedCell))
#define DerivedCellFree(junk) SizeFree(junk, sizeof(DerivedCell))

Derived_p DerivedAlloc(void);
#define DerivedFree(junk) DerivedCellFree(junk)

void DerivationStackPCLPrint(FILE* out, Sig_p sig, PStack_p derivation);
void DerivationStackTSTPPrint(FILE* out, Sig_p sig, PStack_p derivation);

void DerivedPCLPrint(FILE* out, Sig_p sig, Derived_p derived);
void DerivedTSTPPrint(FILE* out, Sig_p sig, Derived_p derived);

#define DerivationCellAlloc() (DerivationCell*)SizeMalloc(sizeof(DerivationCell))
#define DerivationCellFree(junk) SizeFree(junk, sizeof(DerivationCell))

Derivation_p DerivationAlloc(Sig_p sig);
void         DerivationFree(Derivation_p junk);

Derived_p DerivationGetDerived(Derivation_p derivation, Clause_p clause,
                               WFormula_p formula);


long DerivationExtract(Derivation_p derivation, PStack_p root_clauses);
long DerivationTopoSort(Derivation_p derivation);
void DerivationRenumber(Derivation_p derivation);

Derivation_p DerivationCompute(PStack_p root_clauses, Sig_p sig);
void DerivationPrint(FILE* out, Derivation_p derivation, char* frame);


#endif

/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/





