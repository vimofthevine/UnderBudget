#!/bin/sh
export UNDERBUDGET_HOME=./data
$JAVA_HOME/bin/java \
    -Djava.util.logging.config.file=$UNDERBUDGET_HOME/logging.properties \
    -jar UnderBudgetCli.jar "$@"
