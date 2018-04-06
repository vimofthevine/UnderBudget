#!/usr/bin/env python3
import argparse
import sys

from PyQt5.QtWidgets import QApplication

from underbudget.app import MainWindow


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Personal finance and budget manager')
    args = parser.parse_args()

    app = QApplication(sys.argv)
    app.setApplicationName('UnderBudget-py')
    app.setApplicationVersion('4.0.0-dev-py')
    app.setOrganizationName('vimofthevine')
    app.setOrganizationDomain('underbudget.vimofthevine.com')

    window = MainWindow()
    window.show()

    app.exec()
