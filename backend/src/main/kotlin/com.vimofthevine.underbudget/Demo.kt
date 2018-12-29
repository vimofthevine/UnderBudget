package com.vimofthevine.underbudget

import com.vimofthevine.underbudget.auth.*
import com.vimofthevine.underbudget.ledger.*

import org.joda.time.DateTime
import org.slf4j.LoggerFactory

fun setupDemo() {
    val logger = LoggerFactory.getLogger("underbudget.demo")
    logger.info("Populating DB with demo data")
    
    val now = DateTime()
    val usd = Currencies.get("USD")
    
    val demoLedger = Ledger.new {
        name = "Demo Ledger"
        defaultCurrency = usd
        time = now
    }
    
    val rootAccount = Account.new {
        name = "Root"
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val creditCards = Account.new {
        name = "Credit Cards"
        parent = rootAccount
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
        parent = rootAccount
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val rootEnvelope = Envelope.new {
        name = "Root"
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val food = Envelope.new {
        name = "Food"
        parent = rootEnvelope
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val utilities = Envelope.new {
        name = "Utilities"
        parent = rootEnvelope
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
        parent = rootEnvelope
        ledger = demoLedger
        currency = usd
        time = now
    }
    
    val unallocated = Envelope.new {
        name = "Unallocated"
        parent = rootEnvelope
        ledger = demoLedger
        currency = usd
        time = now
    }
}