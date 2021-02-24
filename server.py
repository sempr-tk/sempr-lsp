#!/usr/bin/env python

from pygls.features import COMPLETION, TEXT_DOCUMENT_DID_OPEN, TEXT_DOCUMENT_DID_CHANGE
from pygls.server import LanguageServer
from pygls.types import *
from pygls.workspace import *

import semprlsp

import sys

ruleChecker = semprlsp.RuleChecker()
server = LanguageServer()

conditionsAndEffects = set()
for c in ruleChecker.listConditions():
    conditionsAndEffects.add(c)
for e in ruleChecker.listEffects():
    conditionsAndEffects.add(e)
completionItems = [CompletionItem(i, kind=CompletionItemKind.Function) for i in conditionsAndEffects]

@server.feature(COMPLETION, trigger_characters=[])
def completions(params: CompletionParams):

    return CompletionList(False, completionItems);



def get_diagnostics(rules):
    errs = ruleChecker.getErrors(rules)
    dias = []
    for err in errs:
        diagnostic = Diagnostic(
            range=Range(Position(line=err.line, character=err.start),
                        Position(line=err.line, character=err.end)),
            message=err.description,
            severity=DiagnosticSeverity.Error,
            source="semprlsp"
        )
        dias.append(diagnostic)

    return dias


@server.feature(TEXT_DOCUMENT_DID_CHANGE)
def did_change(ls, params: DidChangeTextDocumentParams):
    text_doc = ls.workspace.get_document(params.textDocument.uri)

    rulestring = ""
    for i,l in enumerate(text_doc.lines):
        rulestring += l

    dias = get_diagnostics(rulestring)
    ls.publish_diagnostics(text_doc.uri, dias)

@server.feature(TEXT_DOCUMENT_DID_OPEN)
def did_open(ls, params : DidOpenTextDocumentParams):
    dias = get_diagnostics(params.textDocument.text)
    ls.publish_diagnostics(params.textDocument.uri, dias)

print("starting server")
server.start_tcp('localhost', 8066)
#server.start_io()
