package com.vimofthevine.underbudget

data class Language(val name: String, val hotness : Int)

class HelloWorld {
    fun kotlinLang() = Language("Kotlin", 10)
}

fun main(args: Array<String>) {
    println(HelloWorld().kotlinLang().name)
}