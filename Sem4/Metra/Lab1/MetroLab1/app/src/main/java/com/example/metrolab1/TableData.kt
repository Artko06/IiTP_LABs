package com.example.metrolab1

import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import kotlin.math.log2

data class TableRow(
    val index: Int,
    val operator: String?,
    val operatorFrequency: Int?,
    val operand: String?,
    val operandFrequency: Int?
)

@Composable
fun HalsteadMetricsTable(
    data: List<TableRow>,
    countOperators: Int,
    countOperands: Int,
    countUniqueOperators: Int,
    countUniqueOperands: Int,
    modifier: Modifier = Modifier
) {

    Column(modifier = modifier) {
        // Заголовок таблицы
        Row(Modifier.fillMaxWidth().border(2.dp, Color.Black)) {
            TableCell(text = "j", modifier = Modifier.weight(1f))
            TableCell(text = "Оператор", modifier = Modifier.weight(2f))
            TableCell(text = "f₁ⱼ", modifier = Modifier.weight(1f))
            TableCell(text = "i", modifier = Modifier.weight(1f))
            TableCell(text = "Операнд", modifier = Modifier.weight(2f))
            TableCell(text = "f₂ᵢ", modifier = Modifier.weight(1f))
        }

        // Данные таблицы
        LazyColumn {
            items(data) { row ->
                Row(Modifier.fillMaxWidth().border(2.dp, Color.Black)) {
                    TableCell(text = row.index.toString() + ".", modifier = Modifier.weight(1f))
                    TableCell(text = row.operator ?: "", modifier = Modifier.weight(2f))
                    TableCell(text = row.operatorFrequency?.toString() ?: "", modifier = Modifier.weight(1f))
                    TableCell(text = row.index.toString() + ".", modifier = Modifier.weight(1f))
                    TableCell(text = row.operand ?: "", modifier = Modifier.weight(2f))
                    TableCell(text = row.operandFrequency?.toString() ?: "", modifier = Modifier.weight(1f))
                }
            }

            item {
                Spacer(modifier = Modifier.height(16.dp)) // Отступ для улучшения компоновки
                Text(text = "Общее число операторов программы(N1): $countOperators", fontSize = 12.sp)
                Text(text = "Общее число операнд в программ(N2): $countOperands", fontSize = 12.sp)
                Text(text = "Длина программы(N = N1 + N2): ${countOperators + countOperands}", fontSize = 12.sp)
                Text(text = "Число уникальных операторов программы(ɳ1): $countUniqueOperators", fontSize = 12.sp)
                Text(text = "Число уникальных операнд программы(ɳ2): $countUniqueOperands", fontSize = 12.sp)
                Text(text = "Cловарь программы(ɳ): ${countUniqueOperators + countUniqueOperands}", fontSize = 12.sp)
                Text(text = "Объем программы(V = Nlog₂ɳ): " +
                        "${(countOperators + countOperands) * log2((countUniqueOperators + countUniqueOperands).toFloat())}",
                    fontSize = 12.sp)
            }
        }
    }
}

@Composable
fun TableCell(
    text: String,
    modifier: Modifier = Modifier
) {
    Box(
        modifier = modifier.padding(8.dp),
        contentAlignment = Alignment.Center
    ) {
        Text(
            text = text,
            fontWeight = FontWeight.Medium,
            fontSize = 14.sp,
            textAlign = TextAlign.Center,
        )
    }
}
