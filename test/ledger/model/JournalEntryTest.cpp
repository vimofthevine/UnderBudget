// Standard include(s)
#include <memory>
#include <vector>

// Qt include(s)
#include <QtCore>

// Google include(s)
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// UnderBudget include(s)
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/Currency.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>
#include <ledger/model/JournalEntry.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/model/Transaction.hpp>
#include <ledger/model/TransactionRepository.hpp>

namespace ub {
namespace ledger {

/** Mock transaction repository */
class MockTransactionRepository : public TransactionRepository {
public:
    MOCK_METHOD1(create, int(const AccountTransaction &));
    MOCK_METHOD1(create, int(const EnvelopeTransaction &));
    MOCK_METHOD1(create, int(const Transaction &));
    MOCK_METHOD1(getAccountTransaction, AccountTransaction(int));
    MOCK_METHOD1(getAccountTransactions, std::vector<AccountTransaction>(const Transaction &));
    MOCK_METHOD2(getBalance, Money(const QDate &, const Account &));
    MOCK_METHOD2(getBalance, Money(const QDate &, const Envelope &));
    MOCK_METHOD1(getEnvelopeTransaction, EnvelopeTransaction(int));
    MOCK_METHOD1(getEnvelopeTransactions, std::vector<EnvelopeTransaction>(const Transaction &));
    MOCK_METHOD2(getReconciledBalance, Money(const QDate &, const Account &));
    MOCK_METHOD1(getTransaction, Transaction(int));
    MOCK_METHOD1(getTransactions, std::vector<AccountTransaction>(const Account &));
    MOCK_METHOD1(getTransactions, std::vector<EnvelopeTransaction>(const Envelope &));
    MOCK_METHOD0(lastError, QString());
    MOCK_METHOD1(remove, bool(const Transaction &));
    MOCK_METHOD1(remove, bool(const AccountTransaction &));
    MOCK_METHOD1(remove, bool(const EnvelopeTransaction &));
    MOCK_METHOD0(save, bool());
    MOCK_METHOD1(update, bool(const Transaction &));
    MOCK_METHOD1(update, bool(const AccountTransaction &));
    MOCK_METHOD1(update, bool(const EnvelopeTransaction &));
};

/** Test fixture */
class JournalEntryTest : public ::testing::Test {
protected:
    /** Mock transaction repository */
    std::shared_ptr<MockTransactionRepository> repo;

    /** Sets up the mock repository */
    void SetUp() override {
        repo.reset(new MockTransactionRepository);
    }

    /** Sets up the mock repository so that it will return existing transactions. */
    void populate() {
        Transaction transaction(80);
        transaction.setPayee("Grocer");
        transaction.setDate(QDate(2017, 1, 3));
        EXPECT_CALL(*repo, getTransaction(80))
            .WillOnce(::testing::Return(transaction));

        AccountTransaction from(3);
        from.setMemo("From Bank");
        from.setAmount(Money(-50.00, Currency(1, "USD")));
        std::vector<AccountTransaction> acct_splits{from};
        EXPECT_CALL(*repo, getAccountTransactions(::testing::_))
            .WillOnce(::testing::Return(acct_splits));

        EnvelopeTransaction to1(6);
        to1.setMemo("Part 1");
        to1.setAmount(Money(-20.00, Currency(1, "USD")));
        EnvelopeTransaction to2(7);
        to2.setMemo("Part 2");
        to2.setAmount(Money(-30.00, Currency(1, "USD")));
        std::vector<EnvelopeTransaction> env_splits{to1, to2};
        EXPECT_CALL(*repo, getEnvelopeTransactions(::testing::_))
            .WillOnce(::testing::Return(env_splits));
    }
};

/** Verifies that the entry is not valid if it has no splits. */
TEST_F(JournalEntryTest, ShouldBeInvalidWhenNoSplitsDefined) {
    JournalEntry entry(repo);
    EXPECT_FALSE(entry.isValid());
}

/** Verifies that the entry is not valid if it has multiple account and envelope splits. */
TEST_F(JournalEntryTest, ShouldBeInvalidWhenMultipleAccountAndMultipleEnvelopeSplitsDefined) {
    JournalEntry entry(repo);
    entry.addSplit(AccountTransaction(2));
    entry.addSplit(AccountTransaction(4));
    entry.addSplit(EnvelopeTransaction(3));
    entry.addSplit(EnvelopeTransaction(5));
    EXPECT_FALSE(entry.isValid());
}

/** Verifies that the entry is not valid if multiple currencies exist in the account splits. */
TEST_F(JournalEntryTest, ShouldBeInvalidWhenMultipleCurrenciesDefinedInAccountSplits) {
    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    AccountTransaction to;
    to.setAmount(Money(12.35, Currency(2, "UAH")));
    entry.addSplit(to);

    EXPECT_FALSE(entry.isValid());
}

/** Verifies that the entry is not valid if multiple currencies exist in the envelope splits. */
TEST_F(JournalEntryTest, ShouldBeInvalidWhenMultipleCurrenciesDefinedInEnvelopeSplits) {
    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to;
    to.setAmount(Money(-12.35, Currency(2, "UAH")));
    entry.addSplit(to);

    EXPECT_FALSE(entry.isValid());
}

/** Verifies that the entry is not valid if it does not have a zero sum. */
TEST_F(JournalEntryTest, ShouldBeInvalidWhenSumIsNotZero) {
    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to;
    to.setAmount(Money(-12.34, Currency(1, "USD")));
    entry.addSplit(to);

    EXPECT_FALSE(entry.isValid()) << entry.lastError().toStdString();
}

/** Verifies that the transaction is not saved in the repository when not valid. */
TEST_F(JournalEntryTest, ShouldNotSaveWhenNotValid) {
    EXPECT_CALL(*repo, save()).Times(0);

    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to;
    to.setAmount(Money(-12.34, Currency(2, "UAH")));
    entry.addSplit(to);

    EXPECT_FALSE(entry.save());
}

/** Verifies that the entry is valid when all conditions are met. */
TEST_F(JournalEntryTest, ShouldBeValidWhenAllConditionsAreSatisfied) {
    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to1;
    to1.setAmount(Money(-10.00, Currency(1, "USD")));
    entry.addSplit(to1);

    EnvelopeTransaction to2;
    to2.setAmount(Money(-2.35, Currency(1, "USD")));
    entry.addSplit(to2);

    EXPECT_TRUE(entry.isValid());
}

/** Verifies that account splits can be removed from an entry. */
TEST_F(JournalEntryTest, ShouldRemoveAccountSplitsFromEntry) {
    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    AccountTransaction to;
    to.setAmount(Money(12.35, Currency(1, "USD")));
    entry.addSplit(to);

    ASSERT_EQ(2u, entry.getAccountSplits().size());

    entry.removeSplit(from);
    ASSERT_EQ(1u, entry.getAccountSplits().size());
    EXPECT_EQ(Money(12.35, Currency(1, "USD")),
              entry.getAccountSplits()[0].amount());
}

/** Verifies that envelope splits can be removed from an entry. */
TEST_F(JournalEntryTest, ShouldRemoveEnvelopeSplitsFromEntry) {
    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to1;
    to1.setAmount(Money(-10.00, Currency(1, "USD")));
    entry.addSplit(to1);

    EnvelopeTransaction to2;
    to2.setAmount(Money(-2.35, Currency(1, "USD")));
    entry.addSplit(to2);

    ASSERT_EQ(2u, entry.getEnvelopeSplits().size());

    entry.removeSplit(to1);
    ASSERT_EQ(1u, entry.getEnvelopeSplits().size());
    EXPECT_EQ(Money(-2.35, Currency(1, "USD")),
              entry.getEnvelopeSplits()[0].amount());
}

/** Verifies that account splits can be updated in an entry. */
TEST_F(JournalEntryTest, ShouldUpdateAccountSplits) {
    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    AccountTransaction to;
    to.setAmount(Money(12.35, Currency(1, "USD")));
    entry.addSplit(to);

    EXPECT_EQ(Money(12.35, Currency(1, "USD")),
              entry.getAccountSplits()[1].amount());


    to.setAmount(Money(22.35, Currency(1, "USD")));
    entry.updateSplit(to, 1);

    EXPECT_EQ(Money(22.35, Currency(1, "USD")),
              entry.getAccountSplits()[1].amount());
}

/** Verifies that envelope splits can be updated in an entry. */
TEST_F(JournalEntryTest, ShouldUpdateEnvelopeSplits) {
    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to1;
    to1.setAmount(Money(-10.00, Currency(1, "USD")));
    entry.addSplit(to1);

    EnvelopeTransaction to2;
    to2.setAmount(Money(-2.35, Currency(1, "USD")));
    entry.addSplit(to2);

    EXPECT_EQ(Money(-10.00, Currency(1, "USD")),
              entry.getEnvelopeSplits()[0].amount());


    to1.setAmount(Money(-12.00, Currency(1, "USD")));
    entry.updateSplit(to1, 0);

    EXPECT_EQ(Money(-12.00, Currency(1, "USD")),
              entry.getEnvelopeSplits()[0].amount());
}

/** Verifies that new entries create transactions in the repository. */
TEST_F(JournalEntryTest, ShouldCreateTransactionsInRepoWhenNewEntry) {
    using namespace ::testing;
    EXPECT_CALL(*repo, create(A<const Transaction &>())).WillOnce(Return(1));
    EXPECT_CALL(*repo, create(A<const AccountTransaction &>())).WillOnce(Return(1));
    EXPECT_CALL(*repo, create(A<const EnvelopeTransaction &>())).WillOnce(Return(1));
    EXPECT_CALL(*repo, getTransaction(::testing::_));
    EXPECT_CALL(*repo, save()).WillOnce(Return(true));

    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to;
    to.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(to);

    EXPECT_TRUE(entry.save());
}

/** Verifies that new entries are not created when error creating an account transaction. */
TEST_F(JournalEntryTest, ShouldNotCreateTransactionsInRepoWhenErrorCreatingAccountTransaction) {
    using namespace ::testing;
    EXPECT_CALL(*repo, create(A<const Transaction &>())).WillOnce(Return(1));
    EXPECT_CALL(*repo, getTransaction(1));
    EXPECT_CALL(*repo, create(A<const AccountTransaction &>())).WillOnce(Return(-1));
    EXPECT_CALL(*repo, lastError()).WillOnce(Return("Error"));
    EXPECT_CALL(*repo, save()).Times(0);

    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to;
    to.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(to);

    EXPECT_FALSE(entry.save());
}

/** Verifies that new entries are not created when error creating an envelope transaction. */
TEST_F(JournalEntryTest, ShouldNotCreateTransactionsInRepoWhenErrorCreatingEnvelopeTransaction) {
    using namespace ::testing;
    EXPECT_CALL(*repo, create(A<const Transaction &>())).WillOnce(Return(1));
    EXPECT_CALL(*repo, getTransaction(1));
    EXPECT_CALL(*repo, create(A<const AccountTransaction &>())).WillOnce(Return(1));
    EXPECT_CALL(*repo, create(A<const EnvelopeTransaction &>())).WillOnce(Return(-1));
    EXPECT_CALL(*repo, lastError()).WillOnce(Return("Error"));
    EXPECT_CALL(*repo, save()).Times(0);

    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to;
    to.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(to);

    EXPECT_FALSE(entry.save());
}

/** Verifies that new entries are not created when error creating a double-entry transaction. */
TEST_F(JournalEntryTest, ShouldNotCreateTransactionsInRepoWhenErrorCreatingDoubleEntryTransaction) {
    using namespace ::testing;
    EXPECT_CALL(*repo, create(A<const Transaction &>())).WillOnce(Return(-1));
    EXPECT_CALL(*repo, lastError()).WillOnce(Return("Error"));
    EXPECT_CALL(*repo, save()).Times(0);

    JournalEntry entry(repo);

    AccountTransaction from;
    from.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(from);

    EnvelopeTransaction to;
    to.setAmount(Money(-12.35, Currency(1, "USD")));
    entry.addSplit(to);

    EXPECT_FALSE(entry.save());
}

/** Verifies that the entry can be initialized to an existing transaction. */
TEST_F(JournalEntryTest, ShouldInitializeWithExistingTransaction) {
    populate();
    JournalEntry entry(repo, Transaction(80));

    EXPECT_EQ(QString("Grocer"), entry.getTransaction().payee());
    EXPECT_EQ(QString("From Bank"), entry.getAccountSplits()[0].memo());
    EXPECT_EQ(QString("Part 1"), entry.getEnvelopeSplits()[0].memo());
    EXPECT_EQ(QString("Part 2"), entry.getEnvelopeSplits()[1].memo());
}

/** Verifies that the entry will update an existing transaction in the repository. */
TEST_F(JournalEntryTest, ShouldUpdateTransactionInRepoWhenModifyingExisting) {
    using namespace ::testing;
    EXPECT_CALL(*repo, update(Matcher<const AccountTransaction &>(
                                  AllOf(
                                      Property(&AccountTransaction::id, 3),
                                      Property(&AccountTransaction::memo, "U1")))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const EnvelopeTransaction &>(
                                  AllOf(
                                      Property(&EnvelopeTransaction::id, 6),
                                      Property(&EnvelopeTransaction::memo, "U2")))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const EnvelopeTransaction &>(
                                  AllOf(
                                      Property(&EnvelopeTransaction::id, 7),
                                      Property(&EnvelopeTransaction::memo, "U3")))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const Transaction &>(
                                  AllOf(
                                      Property(&Transaction::id, 80),
                                      Property(&Transaction::payee, "U4")))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, save()).WillOnce(Return(true));

    populate();
    JournalEntry entry(repo, Transaction(80));

    auto as = entry.getAccountSplits()[0];
    as.setMemo("U1");
    entry.updateSplit(as, 0);

    auto es0 = entry.getEnvelopeSplits()[0];
    es0.setMemo("U2");
    entry.updateSplit(es0, 0);

    auto es1 = entry.getEnvelopeSplits()[1];
    es1.setMemo("U3");
    entry.updateSplit(es1, 1);

    auto t = entry.getTransaction();
    t.setPayee("U4");
    entry.updateTransaction(t);

    entry.save();
}

/** Verifies that the entry will duplicate an existing transaction in the repository. */
TEST_F(JournalEntryTest, ShouldCreateTransactionInRepoWhenDuplicatingExisting) {
    using namespace ::testing;
    EXPECT_CALL(*repo, create(A<const Transaction &>())).WillOnce(Return(3));
    EXPECT_CALL(*repo, getTransaction(3));
    EXPECT_CALL(*repo, create(A<const AccountTransaction &>())).WillOnce(Return(2));
    EXPECT_CALL(*repo, create(A<const EnvelopeTransaction &>())).Times(2)
        .WillRepeatedly(Return(4));
    EXPECT_CALL(*repo, save()).WillOnce(Return(true));

    populate();
    JournalEntry entry(repo, Transaction(80), true);
    entry.save();
}

/** Verifies that the entry is not saved if an error occurs updating an account transaction. */
TEST_F(JournalEntryTest, ShouldNotUpdateTransactionWhenErrorUpdatingAccountTransaction) {
    using namespace ::testing;
    EXPECT_CALL(*repo, update(Matcher<const Transaction &>(
                                  Property(&Transaction::id, 80))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const AccountTransaction &>(
                                  Property(&AccountTransaction::id, 3))))
            .WillOnce(Return(false));
    EXPECT_CALL(*repo, lastError()).WillOnce(Return("Error"));
    EXPECT_CALL(*repo, save()).Times(0);

    populate();
    JournalEntry entry(repo, Transaction(80));
    entry.save();
}

/** Verifies that the entry is not saved if an error occurs updating an envelope transaction. */
TEST_F(JournalEntryTest, ShouldNotUpdateTransactionWhenErrorUpdatingEnvelopeTransaction) {
    using namespace ::testing;
    EXPECT_CALL(*repo, update(Matcher<const Transaction &>(
                                  Property(&Transaction::id, 80))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const AccountTransaction &>(
                                      Property(&AccountTransaction::id, 3))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const EnvelopeTransaction &>(
                                  Property(&EnvelopeTransaction::id, 6))))
            .WillOnce(Return(false));
    EXPECT_CALL(*repo, lastError()).WillOnce(Return("Error"));
    EXPECT_CALL(*repo, save()).Times(0);

    populate();
    JournalEntry entry(repo, Transaction(80));
    entry.save();
}

/** Verifies that the entry is not saved if an error occurs updating the double-entry transaction. */
TEST_F(JournalEntryTest, ShouldNotUpdateTransactionWhenErrorUpdatingDoubleEntryTransaction) {
    using namespace ::testing;
    EXPECT_CALL(*repo, update(Matcher<const Transaction &>(
                                  Property(&Transaction::id, 80))))
            .WillOnce(Return(false));
    EXPECT_CALL(*repo, lastError()).WillOnce(Return("Error"));
    EXPECT_CALL(*repo, save()).Times(0);

    populate();
    JournalEntry entry(repo, Transaction(80));
    entry.save();
}

/** Verifies that account transactions can be removed from transactions. */
TEST_F(JournalEntryTest, ShouldRemoveDeletedAccountTransactionsFromRepository) {
    using namespace ::testing;
    EXPECT_CALL(*repo, create(A<const AccountTransaction &>()))
            .WillOnce(Return(2));
    EXPECT_CALL(*repo, remove(Matcher<const AccountTransaction &>(
                                  Property(&AccountTransaction::id, 3))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const EnvelopeTransaction &>(
                                  Property(&EnvelopeTransaction::id, 6))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const EnvelopeTransaction &>(
                                  Property(&EnvelopeTransaction::id, 7))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const Transaction &>(
                                  Property(&Transaction::id, 80))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, save()).WillOnce(Return(true));

    populate();
    JournalEntry entry(repo, Transaction(80));

    entry.removeSplit(entry.getAccountSplits()[0]);

    // Have to maintain zero-sum
    AccountTransaction new_acct;
    new_acct.setAmount(Money(-50.00, Currency(1, "USD")));
    entry.addSplit(new_acct);

    EXPECT_TRUE(entry.save()) << entry.lastError().toStdString();
}

/** Verifies that envelope transactions can be removed from transactions. */
TEST_F(JournalEntryTest, ShouldRemoveDeletedEnvelopeTransactionsFromRepository) {
    using namespace ::testing;
    EXPECT_CALL(*repo, update(Matcher<const AccountTransaction &>(
                                  Property(&AccountTransaction::id, 3))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, remove(Matcher<const EnvelopeTransaction &>(
                                  Property(&EnvelopeTransaction::id, 7))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const EnvelopeTransaction &>(
                                  Property(&EnvelopeTransaction::id, 6))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, update(Matcher<const Transaction &>(
                                  Property(&Transaction::id, 80))))
            .WillOnce(Return(true));
    EXPECT_CALL(*repo, save()).WillOnce(Return(true));

    populate();
    JournalEntry entry(repo, Transaction(80));

    entry.removeSplit(entry.getEnvelopeSplits()[1]);

    // Have to maintain zero-sum
    auto split = entry.getEnvelopeSplits()[0];
    split.setAmount(Money(-50.0, Currency(1, "USD")));
    entry.updateSplit(split, 0);

    EXPECT_TRUE(entry.save()) << entry.lastError().toStdString();
}

} // ledger namespace
} // ub namespace
