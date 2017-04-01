# UnderBudget

UnderBudget is an open-source personal budget analysis application.

* Source : [Bitbucket][source] ([GitHub mirror][github])
* [User Guide][guide]
* [Issue Tracker][issues]
* [Wiki][wiki]
* [Discussion Forums][forums]
* Continuous Integration : [![Build Status](https://secure.travis-ci.org/vimofthevine/UnderBudget.png)][travis]

## Features

UnderBudget allows you to create a personal budget and track your ongoing
progress (so that you stay *under-budget*) by comparing against your actual
expenses.

* Categorizing of estimates into a hierarchy (e.g., Expenses->Food->Groceries
  and Expenses->Food->Dining)
* Ability to import transaction data from GnuCash
* Assignment of transactions to estimates through flexible user-defined rules
* Calculation of:
    * *estimated* ending balance (initial balance + estimated incomes - estimated expenses)
    * *actual* ending balance (initial balance + actual income to-date - actual expenses to-date)
    * *expected* ending balance (actual ending balance, taking into account estimates that have already gone over-budget)
* Uses the [Qt framework](http://qt-project.org) to run on Linux, OS X,
  and Windows

![UnderBudget](http://vimofthevine.com/assets/img/underbudget_screen.png)

## Installation

Start by [downloading][download] the appropriate package for your operating system
and follow the [installation guide][install].

UnderBudget can be built from the source by [cloning][source] the repository and
following the [build procedure][build].

## Contributing

Contributions are welcome in all forms.

* Found a bug? Please submit a [bug report][issues]!
* Want to help write the [user manual][guide]?
* [Fork the source][fork] and submit pull requests to help make UnderBudget better!
    * [Developer's Guidelines][guidelines]
    * [Design Documentation][design]

## License

UnderBudget is written by Kyle Treubig and released under the [GNU General Public License,
version 3][gplv3].

Versions prior to 4.0 were released under the [Apache 2.0 license][apache]. However, as
the QtCharts module is licensed under GPLv3 and not LGPLv3, the license was changed to comply.

[github]: https://github.com/vimofthevine/UnderBudget
[source]: https://bitbucket.org/vimofthevine/underbudget/src
[issues]: https://bitbucket.org/vimofthevine/underbudget/issues
[forums]: http://forums.underbudget.vimofthevine.com
[wiki]: https://bitbucket.org/vimofthevine/underbudget/wiki
[guide]: https://bitbucket.org/vimofthevine/underbudget/wiki/guide
[install]: https://bitbucket.org/vimofthevine/underbudget/wiki/guide/installation
[build]: https://bitbucket.org/vimofthevine/underbudget/wiki/guide/building
[guidelines]: https://bitbucket.org/vimofthevine/underbudget/wiki/guidelines
[design]: https://bitbucket.org/vimofthevine/underbudget/wiki/design
[fork]: https://bitbucket.org/vimofthevine/underbudget/fork
[download]: https://bitbucket.org/vimofthevine/underbudget/downloads
[travis]: http://travis-ci.org/vimofthevine/UnderBudget
[apache]: http://www.apache.org/licenses/LICENSE-2.0.html
[gplv3]: http://www.gnu.org/licenses/gpl.html
