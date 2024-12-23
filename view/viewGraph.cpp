#include "viewGraph.h"

String viewGraph::html = "";
String viewGraph::generateHTML()
{
    html = viewGeneric::defaultCssHeader("Graph View");

    html += "<h1>Graph View</h1>";

    // Menu a tendina per selezionare l'indirizzo del registro
    html += "<label for='           register-select'>Select Register Address: or gpio value</label>";
    html += "<select id='register-select' onchange='updateGraph()'>";

    std::vector<int> addresses = SystemState::getInstance()->getAllRegisterAddress();
    String apiFetch = "getRegisterValues";
    String apiFetchParam = "address";

    html += generateGraph(addresses, apiFetch, apiFetchParam);

    html += viewGeneric::defaultFooter();

    return html;
}


String viewGraph::generateGraph(std::vector<int> addresses, String apiFetch, String apiFetchParam)
{
    String var_html = "";

    for (int addr : addresses)
    {
        if (addr > 0)
        {
            var_html += "<option value='" + String(addr) + "'>" + String(addr) + "</option>";
        }
        else
        {
            var_html += "<option value='" + String(addr) + "'>GPIO:" + String(-addr) + "</option>";
        }
    }

    var_html += "</select>";

    // Contenitore per il grafico
    var_html += "<canvas id='myChart' width='400' height='200'></canvas>";

    // Script JavaScript per Chart.js e per gestire il grafico
    var_html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
    var_html += "<script>";
    var_html += "let chart;";
    var_html += "function updateGraph() {";
    var_html += "  const address = document.getElementById('register-select').value;";
    var_html += "  fetch('/"+apiFetch+"?"+apiFetchParam+"=' + address)";
    var_html += "    .then(response => response.json())";
    var_html += "    .then(data => {";
    var_html += "      const labels = data.map((_, index) => index + 1);";
    var_html += "      const values = data;";
    var_html += "      const ctx = document.getElementById('myChart').getContext('2d');";
    var_html += "      if (chart) chart.destroy();";
    var_html += "      chart = new Chart(ctx, {";
    var_html += "        type: 'line',";
    var_html += "        data: {";
    var_html += "          labels: labels,";
    var_html += "          datasets: [{";
    var_html += "            label: 'Register Values',";
    var_html += "            data: values,";
    var_html += "            borderColor: 'rgba(75, 192, 192, 1)',";
    var_html += "            borderWidth: 2";
    var_html += "          }]";
    var_html += "        },";
    var_html += "        options: {";
    var_html += "          responsive: true,";
    var_html += "          animation: false,"; // Disabilita l'animazione
    var_html += "          scales: {";
    var_html += "            x: {";
    var_html += "              display: true,";
    var_html += "              title: {";
    var_html += "                display: true,";
    var_html += "                text: 'Sample Index'";
    var_html += "              }";
    var_html += "            },";
    var_html += "            y: {";
    var_html += "              display: true,";
    var_html += "              title: {";
    var_html += "                display: true,";
    var_html += "                text: 'Value'";
    var_html += "              }";
    var_html += "            }";
    var_html += "          }";
    var_html += "        }";
    var_html += "      });";
    var_html += "    });";
    var_html += "}";
    var_html += "document.addEventListener('DOMContentLoaded', () => {";
    var_html += "  updateGraph();";                  // Per caricare il grafico all'avvio
    var_html += "  setInterval(updateGraph, 3000);"; // Aggiorna il grafico ogni 3 secondi
    var_html += "});";
    var_html += "</script>";

    return var_html;
}