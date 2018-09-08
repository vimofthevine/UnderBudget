#!/usr/bin/env python3
import argparse
import sys

from PyQt5.QtWidgets import QApplication

from underbudget import demo
from underbudget.ui import MainWindow


__version__ = '4.0.0-dev-py'


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Personal finance and budget manager')
    parser.add_argument('--demo', action='store_true', help='Open an in-memory demo database')
    parser.add_argument('--version', action='version',
                        version='UnderBudget {0}'.format(__version__))
    args = parser.parse_args()

    app = QApplication(sys.argv)
    app.setApplicationName('UnderBudget-py')
    app.setApplicationVersion(__version__)
    app.setOrganizationName('vimofthevine')
    app.setOrganizationDomain('underbudget.vimofthevine.com')

    if args.demo:
        demo.setup()

    window = MainWindow()
    window.show()

    app.exec()
