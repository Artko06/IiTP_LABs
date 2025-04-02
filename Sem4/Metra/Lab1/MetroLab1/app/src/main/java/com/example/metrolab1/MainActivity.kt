package com.example.metrolab1

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.gestures.Orientation
import androidx.compose.foundation.gestures.scrollable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import com.example.metrolab1.ui.theme.MetroLab1Theme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            MetroLab1Theme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    val context = LocalContext.current // Получаем контекст

                    // Читаем файл при первой композиции
                    val textFile = remember {
                        context.assets.open("AnalysisProgram.kt").bufferedReader().use { it.readText() }
                    }

                    val scrollState = rememberScrollState()
                    val countOperators = remember { mutableStateOf(0) }
                    val countOperands = remember { mutableStateOf(0) }
                    val countUniqueOperators = remember { mutableStateOf(0) }
                    val countUniqueOperands = remember { mutableStateOf(0) }

                    HalsteadMetrics.analyze(textFile)

                    LaunchedEffect(Unit) {
                        val textFile = context.assets.open("AnalysisProgram.kt").bufferedReader().use { it.readText() }
                        HalsteadMetrics.analyze(textFile)
                        countOperators.value = HalsteadMetrics.getCountOperators()
                        countOperands.value = HalsteadMetrics.getCountOperands()
                        countUniqueOperators.value = HalsteadMetrics.getCountUniqueOperators()
                        countUniqueOperands.value = HalsteadMetrics.getCountUniqueOperands()
                    }

                    Column{
                        HalsteadMetricsTable(
                            data = HalsteadMetrics.getTableData(),
                            countOperands = countOperands.value,
                            countOperators = countOperators.value,
                            countUniqueOperators = countUniqueOperators.value,
                            countUniqueOperands = countUniqueOperands.value,
                            modifier = Modifier.fillMaxWidth().padding(innerPadding).scrollable(scrollState, orientation = Orientation.Vertical)
                        )
                    }

                }
            }
        }
    }
}