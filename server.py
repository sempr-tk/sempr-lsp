#!/usr/bin/env python

from pygls.features import COMPLETION, TEXT_DOCUMENT_DID_OPEN, TEXT_DOCUMENT_DID_CHANGE
from pygls.server import LanguageServer
from pygls.types import *
from pygls.workspace import *

import sys

server = LanguageServer()

@server.feature(COMPLETION, trigger_characters=[','])
def completions(params: CompletionParams):
    return CompletionList(False, [
        CompletionItem('Item1'),
        CompletionItem('Item2'),
        CompletionItem('Item3')
    ])


@server.feature(TEXT_DOCUMENT_DID_CHANGE)
def did_change(ls, params: DidChangeTextDocumentParams):

    print("hello??")

    text_doc = ls.workspace.get_document(params.textDocument.uri)

    dias = []
    for i,l in enumerate(text_doc.lines):
        print("hello?", i, l)
        loc = l.find("error")

        if loc >= 0:
            diagnostic = Diagnostic(
                            range=Range(Position(line=i, character=loc),
                                        Position(line=i, character=loc+5)),
                            message="Found 'error'",
                            severity=DiagnosticSeverity.Error,
                            source="rules validation"
                         )
            dias.append(diagnostic)
            print("found error")

    ls.publish_diagnostics(text_doc.uri, dias)

print("starting server")
server.start_tcp('localhost', 8066)
#server.start_io()
