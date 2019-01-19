package com.vimofthevine.underbudget.ledger

import com.google.gson.Gson
import com.vimofthevine.underbudget.TestFixture

import io.ktor.http.*
import io.ktor.server.testing.*

import java.util.UUID

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

class GetLedgerTest : TestFixture() {
    @Test fun `should reject unauthenticated`() = withServer {
        val id = withDatabase(false) { createLedger(Ledger("Ledger", "USD")) }
        val req = handleRequest() {
            method = HttpMethod.Get
            uri = "/ledgers/${id}"
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldEqual HttpStatusCode.Unauthorized
    }
    
    @Test fun `should reject unauthorized`() = withServer {
        val id = withDatabase(false) { createLedger(Ledger("Ledger", "USD")) }
        val req = handleRequest() {
            method = HttpMethod.Get
            uri = "/ledgers/${id}"
            addHeader("Authorization", "Bearer ${createToken()}")
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldEqual HttpStatusCode.Forbidden
    }
    
    @Test fun `should reject invalid ledger`() = withServer {
        val req = handleRequest() {
            method = HttpMethod.Get
            uri = "/ledgers/${UUID.randomUUID()}"
            addHeader("Authorization", "Bearer ${createToken()}")
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldEqual HttpStatusCode.Forbidden
    }
    
    @Test fun `should respond with ledger`() = withServer {
        val id = withDatabase(false) {
            val id = createLedger(Ledger("Ledger", "USD"))
            createLedgerPermission(id, testUserId)
            id
        }
        val req = handleRequest() {
            method = HttpMethod.Get
            uri = "/ledgers/${id}"
            addHeader("Authorization", "Bearer ${createToken()}")
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.OK
            val response = Gson().fromJson(it.content.shouldNotBeNullOrBlank(), Ledger::class.java)
            response.id shouldEqual id
            response.name shouldEqual "Ledger"
            response.defaultCurrency shouldEqual "USD"
        }
    }
}