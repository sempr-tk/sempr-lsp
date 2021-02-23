/* --------------------------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License. See License.txt in the project root for license information.
 * ------------------------------------------------------------------------------------------ */

import * as net from 'net';
import * as path from 'path';
import * as child_process from "child_process"
import { workspace, ExtensionContext } from 'vscode';

import {
	LanguageClient,
	LanguageClientOptions,
	ServerOptions,
	StreamInfo,
	TransportKind
} from 'vscode-languageclient/node';

let client: LanguageClient;

export function activate(context: ExtensionContext) {
	let connectionInfo = {
		port : 8066
	};

	let serverOptions = () => {
		let socket = net.connect(connectionInfo);
		let result : StreamInfo = {
			writer : socket,
			reader: socket
		};
		return Promise.resolve(result);
	}


	// Options to control the language client
	let clientOptions: LanguageClientOptions = {
		// Register the server for plain text documents
		documentSelector: [{ scheme: 'file', language: 'rules' }],
//		synchronize: {
//			// Notify the server about file changes to '.clientrc files contained in the workspace
//			fileEvents: workspace.createFileSystemWatcher('**/.clientrc')
//		}
	};

	// Create the language client and start the client.
	client = new LanguageClient(
		'sempr-lsp-client',
		'sempr-lsp client',
		serverOptions,
		clientOptions
	);

	// Start the client. This will also launch the server
	client.start();
}

export function deactivate(): Thenable<void> | undefined {
	if (!client) {
		return undefined;
	}
	return client.stop();
}
