/** \file
 * \brief Implementation of a command line based tool to run
 * tests.
 *
 * \author Christoph Schulz, Stephan Beyer, Tilo Wiedera
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the OGDF root directory for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <iostream>
#include <bandit/bandit.h>
#include <resources.h>

using namespace ogdf;

int main(int argc, char **argv)
{
	if(!resourceCheck()) {
		std::cerr << "Could not find the resource directory." << std::endl
		    << "Make sure you run tests from within the OGDF source directory." << std::endl;
		return 1;
	}

	bool verbose = false;
	bool help = false;

	for(int i = 1; i < argc; i++) {
		verbose |= string(argv[i]) == "--ogdf-verbose";
		help |= string(argv[i]) == "--help";
	}

	if(!verbose) {
		Logger::globalLogLevel(Logger::Level::Force);
	}

	int result = bandit::run(argc, argv);

	if(help) {
		cout << "OGDF specific options:" << endl;
		cout << "  --ogdf-verbose\t\tEnable verbose OGDF logging." << endl;
	}

	return result;
}
