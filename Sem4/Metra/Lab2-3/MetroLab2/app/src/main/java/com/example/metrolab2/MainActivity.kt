package com.example.metrolab2

import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import com.chaquo.python.PyObject
import com.chaquo.python.Python
import com.example.metrolab2.ui.theme.MetroLab2Theme
import java.io.File
import java.io.FileOutputStream

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        val python = Python.getInstance()
        val module = python.getModule("jilbMetric")

        // Копируем `kotlin.kt` из assets в internal storage
        val filePath = File(filesDir, "kotlin.kt")
        copyFileFromAssets(this@MainActivity, "kotlin.kt", filePath)

        val metricsListener = module.callAttr("JilbMetricsListener")
        module.callAttr("calculate_jilb_metrics", filePath.absolutePath, metricsListener)

        val absoluteComplexity = metricsListener.get("absolute_complexity_get")!!.toInt()
        val totalOperatorsCount = metricsListener.get("total_operators_count_get")!!.toInt()
        val relativeComplexity = metricsListener.get("relative_complexity_get")!!.toDouble()
        val maxNesting = metricsListener.get("max_nesting_get")!!.toInt()

        val raw = metricsListener.get("operators_get") as PyObject
        val rawList = raw.asList()

        val operatorCounts = rawList.associate {
            val pair = it.asList()
            val key = pair[0].toString()
            val value = pair[1].toInt()
            key to value
        }

        Log.d("DEBUG", "Operators: $operatorCounts")

        setContent {
            MetroLab2Theme {
                Scaffold(
                    modifier = Modifier.fillMaxSize()
                ) { paddingValues ->
                    LazyColumn(
                        modifier = Modifier.fillMaxSize()
                            .padding(paddingValues)
                    ) {
                        item {
                            Text("Абсолютная сложность (CL): $absoluteComplexity")
                            Text("Общее количество операторов: $totalOperatorsCount")
                            Text("Относительная сложность (cl): $relativeComplexity")
                            Text("Максимальный уровень вложенности (CLI): $maxNesting")
                            Spacer(modifier = Modifier.height(12.dp))
                        }

                        items(operatorCounts.toList()) { (operator, count) ->
                            Text("$operator -> $count")
                        }
                    }
                }
            }
        }
    }

}

fun copyFileFromAssets(context: Context, assetFileName: String, outputFile: File) {
    val assetManager = context.assets
    assetManager.open(assetFileName).use { inputStream ->
        FileOutputStream(outputFile).use { outputStream ->
            inputStream.copyTo(outputStream)
        }
    }
}