package com.vimofthevine.underbudget.auth

import io.ktor.application.*
import io.ktor.util.hex

import java.security.SecureRandom

import javax.crypto.SecretKeyFactory
import javax.crypto.spec.PBEKeySpec

const val HASH_LENGTH_BITS = 256
const val HASH_LENGTH_BYTES = HASH_LENGTH_BITS / 8

class Passwords(val iterations: Int) {
    private val random = SecureRandom()
    private val factory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1")
    
    fun generateSalt(): String {
        val salt = ByteArray(HASH_LENGTH_BYTES)
        random.nextBytes(salt)
        return hex(salt)
    }
    
    fun hash(password: String, salt: String): String {
        val spec = PBEKeySpec(password.toCharArray(), hex(salt), iterations, HASH_LENGTH_BITS)
        return hex(factory.generateSecret(spec).getEncoded())
    }
}

fun Application.createPasswords(): Passwords =
	Passwords(environment.config.property("auth.iterations").getString().toInt())
