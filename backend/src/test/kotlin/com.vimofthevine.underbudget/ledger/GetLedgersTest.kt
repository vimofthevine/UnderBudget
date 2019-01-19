package com.vimofthevine.underbudget.ledger

import com.google.gson.Gson
import com.vimofthevine.underbudget.TestFixture

import io.ktor.http.*
import io.ktor.server.testing.*

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

class GetLedgersTest : TestFixture() {
    @Test fun `should reject unauthenticated`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Get
            uri = "/ledgers"
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldEqual HttpStatusCode.Unauthorized
    }
    
    @Test fun `should return empty list when no permitted ledgers`() = withServer {
		withDatabase(false) { createLedger(Ledger("Ledger", "USD")) }
        val req = handleRequest() {
            method = HttpMethod.Get
            uri = "/ledgers"
            addHeader("Authorization", "Bearer ${createToken()}")
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.OK
            val response = Gson().fromJson(it.content.shouldNotBeNullOrBlank(), Ledgers::class.java)
            response.ledgers.size shouldEqual 0
        }
    }
    
    @Test fun `should return all permitted ledgers`() = withServer {
		withDatabase(false) {
            val id1 = createLedger(Ledger("Ledger", "USD"))
            createLedgerPermission(id1, testUserId)
            createLedger(Ledger("Ledger 2", "USD"))
            val id2 = createLedger(Ledger("Ledger B", "UAH"))
            createLedgerPermission(id2, testUserId)
        }
        val req = handleRequest() {
            method = HttpMethod.Get
            uri = "/ledgers"
            addHeader("Authorization", "Bearer ${createToken()}")
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.OK
            val response = Gson().fromJson(it.content.shouldNotBeNullOrBlank(), Ledgers::class.java)
            response.ledgers.size shouldEqual 2
        }
    }
}