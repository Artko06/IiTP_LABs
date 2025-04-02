import kotlin.math.PI
import kotlin.math.abs
import kotlin.math.cos
import kotlin.math.ln
import kotlin.math.log
import kotlin.math.sin
import kotlin.math.sqrt

fun testFun(isTrue: Boolean) {
    var a = 5
    when(isTrue){
        true -> {
            a++
        }
        false -> {
            a--
        }
    }
}

fun main() {
    val a = 10; val b = 3
    val c = 5.5
    val d = 2.0

    val sum = a + b
    val difference = a - b
    val product = a * b
    val quotient = a / b
    val remainder = a % b

    val sumDouble = c + d
    val differenceDouble = c - d
    val productDouble = c * d
    val quotientDouble = c / d

    val andResult = a.and(b)
    val orResult = a.or(b)
    val xorResult = a.xor(b)
    val shiftLeft = a.shl(2)
    val shiftRight = a.shr(1)
    val unsignedShiftRight = a.ushr(1)

    var x = 20
    x += 5
    x -= 2
    x *= 3
    x /= 2
    x %= 4

    var y = 7
    val preIncrement = ++y
    val postIncrement = y++
    val preDecrement = --y
    val postDecrement = y--

    val negation = -a
    val absoluteValue = abs(-c)

    val sqrtValue = sqrt(16.0)
    val logValue = ln(10.0)
    val sinValue = sin(PI / 2)
    val cosValue = cos(0.0)

    if(sqrtValue != logValue){
        print(sqrtValue)
    }d
}

object A {
    val a: String = "Simple class"

    fun input(something: String) {
        repeat(5){
            println(a + something)
        }
    }
}
