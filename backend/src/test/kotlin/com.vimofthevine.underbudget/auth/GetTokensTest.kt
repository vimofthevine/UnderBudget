package com.vimofthevine.underbudget.auth

import com.google.gson.Gson
import com.vimofthevine.underbudget.TestFixture

import io.ktor.http.*
import io.ktor.server.testing.*

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

class GetTokensTest : TestFixture() {
    @Test fun `should reject unauthenticated`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Get
            uri = "/tokens"
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldBe HttpStatusCode.Unauthorized
    }
    
    @Test fun `should return token for user`() = withServer {
        val token = createToken()
        val req = handleRequest {
            method = HttpMethod.Get
            uri = "/tokens"
            addHeader("Authorization", "Bearer ${token}")
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.OK
            val response = Gson().fromJson(it.content.shouldNotBeNullOrBlank(), Tokens::class.java)
            response.tokens.size shouldEqual 1
            jwtSvc.decode(token).getId() shouldEqual response.tokens[0].jwtId
        }
    }
    
    @Test fun `should return all active tokens for user`() = withServer {
        val user1 = createUser("otherUser", "other@test.com", "otherPassword")
        createToken(user1)
        createToken()
        createToken(user1)
        createToken()
        val req = handleRequest {
            method = HttpMethod.Get
            uri = "/tokens"
            addHeader("Authorization", "Bearer ${createToken(user1)}")
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.OK
            val response = Gson().fromJson(it.content.shouldNotBeNullOrBlank(), Tokens::class.java)
            response.tokens.size shouldEqual 3
        }
    }
}