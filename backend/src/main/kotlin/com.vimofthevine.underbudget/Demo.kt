package com.vimofthevine.underbudget

import com.vimofthevine.underbudget.auth.*
import com.vimofthevine.underbudget.ledger.*

import org.jetbrains.exposed.sql.*
import org.joda.time.DateTime
import org.slf4j.LoggerFactory

fun setupDemo(service: DbService) {
    val logger = LoggerFactory.getLogger("underbudget.demo")
    
    /*
    if (Ledgers.select { Ledgers.name eq "Demo Ledger" }.count() > 0) {
        logger.info("DB is already populated with demo data")
        return
    }
    logger.info("Populating DB with demo data")
    
    val now = DateTime()
    val usd = Currencies.get("USD")
    
    val demoLedgerId = service.create(Ledger(name = "Demo Ledger", defaultCurrency = "USD"))
    val demoLedger = LedgerDao.new {
        name = "Demo Ledger from DAO"
        defaultCurrency = usd
        time = now
    }
    
    val creditCards = Account.new {
        name = "Credit Cards"
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val redCard = Account.new {
        name = "Red Card"
        parent = creditCards
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val blueCard = Account.new {
        name = "Blue Card"
        parent = creditCards
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val bank = Account.new {
        name = "Bank"
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val food = Envelope.new {
        name = "Food"
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val utilities = Envelope.new {
        name = "Utilities"
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val rent = Envelope.new {
        name = "Rent"
        parent = utilities
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val electric = Envelope.new {
        name = "Electric"
        parent = utilities
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val gifts = Envelope.new {
        name = "Gifts"
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val unallocated = Envelope.new {
        name = "Unallocated"
        ledger = demoLedger
        currency = usd
        time = now
    }
    */
}