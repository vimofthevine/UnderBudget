package com.vimofthevine.underbudget.auth

import com.google.gson.Gson
import com.vimofthevine.underbudget.TestFixture

import io.ktor.http.*
import io.ktor.server.testing.*

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

class DeleteTokenTest : TestFixture() {
    @Test fun `should reject unauthenticated`() = withServer {
        val token = createToken()
        val decoded = jwtSvc.decode(token)
        val req = handleRequest {
            method = HttpMethod.Delete
            uri = "/tokens/${decoded.getId()}"
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldBe HttpStatusCode.Unauthorized
    }
    
    @Test fun `should delete token`() = withServer {
        val token = createToken()
        val decoded = jwtSvc.decode(token)
        val req = handleRequest {
            method = HttpMethod.Delete
            uri = "/tokens/${decoded.getId()}"
            addHeader("Authorization", "Bearer ${token}")
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldBe HttpStatusCode.OK
    }
    
    @Test fun `should not delete token for other user`() = withServer {
        val user1 = createUser("otherUser", "other@test.com", "otherPassword")
        val decoded = jwtSvc.decode(createToken(user1))
        val req = handleRequest {
            method = HttpMethod.Delete
            uri = "/tokens/${decoded.getId()}"
            addHeader("Authorization", "Bearer ${createToken()}")
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldBe HttpStatusCode.Forbidden
    }
    
    @Test fun `should not delete invalid token`() = withServer {
        val token = createToken()
        val req = handleRequest {
            method = HttpMethod.Delete
            uri = "/tokens/not-a-token-jwt-id"
            addHeader("Authorization", "Bearer ${createToken()}")
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldBe HttpStatusCode.NotFound
    }
    
    @Test fun `should invalidate token after deletion`() = withServer {
        val token = createToken()
        val decoded = jwtSvc.decode(token)
        val req1 = handleRequest {
            method = HttpMethod.Delete
            uri = "/tokens/${decoded.getId()}"
            addHeader("Authorization", "Bearer ${token}")
        }
        
        req1.requestHandled shouldBe true
        req1.response.status() shouldBe HttpStatusCode.OK
        
        val req2 = handleRequest {
            method = HttpMethod.Delete
            uri = "/tokens/${decoded.getId()}"
            addHeader("Authorization", "Bearer ${token}")
        }
        
        req2.requestHandled shouldBe true
        req2.response.status() shouldBe HttpStatusCode.Unauthorized
    }
}
