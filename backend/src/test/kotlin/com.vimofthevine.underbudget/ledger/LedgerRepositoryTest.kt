package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.TestFixture

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

class LedgerRepositoryTest : TestFixture() {
    @Test fun `should create ledger`() = withDatabase {
        val id = createLedger(Ledger(name = "test ledger", defaultCurrency = "UAH"))
        val ledger = findLedgerById(id)
        ledger.shouldNotBeNull()
        ledger.name shouldEqual "test ledger"
        ledger.defaultCurrency shouldEqual "UAH"
    }
    
    @Test fun `should create ledger permission`() = withDatabase {
        val id = createLedger(Ledger(name = "ledger 1", defaultCurrency = "USD"))
        hasLedgerPermission(id, testUserId) shouldBe false
        createLedgerPermission(id, testUserId)
        hasLedgerPermission(id, testUserId) shouldBe true
    }
    
    @Test fun `should delete ledger permission`() = withDatabase {
        val id = createLedger(Ledger(name = "ledger 1", defaultCurrency = "USD"))
        createLedgerPermission(id, testUserId)
        hasLedgerPermission(id, testUserId) shouldBe true
        deleteLedgerPermission(id, testUserId)
        hasLedgerPermission(id, testUserId) shouldBe false
    }
    
    @Test fun `should allow multiple users permission to same ledger`() = withDatabase {
        val id = createLedger(Ledger(name = "ledger 1", defaultCurrency = "USD"))
        val spouseUserId = createUser("spouse", "spouse@test.com", "spouse123").id!!
        createLedgerPermission(id, testUserId)
        createLedgerPermission(id, spouseUserId)
        hasLedgerPermission(id, testUserId) shouldBe true
        hasLedgerPermission(id, spouseUserId) shouldBe true
    }
    
    @Test fun `should find all ledgers for which user has permission`() = withDatabase {
        val id1 = createLedger(Ledger(name = "ledger 1", defaultCurrency = "USD"))
        val id2 = createLedger(Ledger(name = "ledger 2", defaultCurrency = "UAH"))
        findLedgersByUser(testUserId).size shouldEqual 0
        createLedgerPermission(id1, testUserId)
        findLedgersByUser(testUserId).size shouldEqual 1
        createLedgerPermission(id2, testUserId)
        findLedgersByUser(testUserId).size shouldEqual 2
        deleteLedgerPermission(id1, testUserId)
        findLedgersByUser(testUserId).size shouldEqual 1
        deleteLedgerPermission(id2, testUserId)
        findLedgersByUser(testUserId).size shouldEqual 0
    }
}