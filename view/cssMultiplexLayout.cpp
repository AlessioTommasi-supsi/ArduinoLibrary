#include "cssMultiplexLayout.h"

String cssMultiplexLayout::getCSS() {
    return R"(
        <style>
            /* Desktop Layout - Horizontal split */
            @media (min-width: 1024px) {
                .desktop-multiplex-layout {
                    display: grid;
                    grid-template-columns: 300px 1fr;
                    grid-template-rows: auto 1fr;
                    gap: 10px;
                    height: 100vh;
                    padding: 8px;
                    box-sizing: border-box;
                    overflow: hidden;
                }
                
                .multiplex-header {
                    grid-column: 1 / -1;
                    flex-shrink: 0;
                    padding: 6px;
                    background: #f8f9fa;
                    border-radius: 6px;
                    text-align: center;
                }
                
                .multiplex-controls {
                    grid-column: 1;
                    display: flex;
                    flex-direction: column;
                    gap: 8px;
                    padding: 12px;
                    background: white;
                    border: 1px solid #ddd;
                    border-radius: 8px;
                    overflow-y: auto;
                    max-height: calc(100vh - 100px);
                }
                
                .multiplex-graph {
                    grid-column: 2;
                    display: flex;
                    flex-direction: column;
                    min-height: 0;
                    background: white;
                    border: 1px solid #ddd;
                    border-radius: 8px;
                    padding: 10px;
                }
                
                .graph-header {
                    flex-shrink: 0;
                    text-align: center;
                    margin-bottom: 8px;
                }
                
                .graph-container {
                    flex: 1;
                    min-height: 0;
                    position: relative;
                    width: 100%;
                    height: 100%;
                }
                
                .realtime-value {
                    position: absolute;
                    top: 10px;
                    right: 10px;
                    background: rgba(255, 255, 255, 0.9);
                    border: 2px solid #2196F3;
                    border-radius: 8px;
                    padding: 6px 10px;
                    font-weight: bold;
                    font-size: 14px;
                    color: #2196F3;
                    z-index: 100;
                    box-shadow: 0 2px 8px rgba(0,0,0,0.1);
                }
                
                #myChart {
                    width: 100% !important;
                    height: 100% !important;
                    min-height: 250px;
                    border: 1px solid #ccc;
                    border-radius: 4px;
                }
            }
            
            /* Mobile Layout - Vertical with iOS toggles */
            @media (max-width: 1023px) {
                .desktop-multiplex-layout {
                    display: flex;
                    flex-direction: column;
                    height: 100vh;
                    padding: 8px;
                    box-sizing: border-box;
                    overflow: hidden;
                }
                
                .multiplex-header {
                    flex-shrink: 0;
                    padding: 6px;
                    background: #f8f9fa;
                    border-radius: 6px;
                    text-align: center;
                    margin-bottom: 8px;
                }
                
                .multiplex-controls {
                    flex-shrink: 0;
                    display: flex;
                    flex-direction: column;
                    gap: 8px;
                    padding: 10px;
                    background: white;
                    border: 1px solid #ddd;
                    border-radius: 8px;
                    margin-bottom: 8px;
                    max-height: 200px;
                    overflow-y: auto;
                }
                
                .multiplex-graph {
                    flex: 1;
                    display: flex;
                    flex-direction: column;
                    min-height: 0;
                    background: white;
                    border: 1px solid #ddd;
                    border-radius: 8px;
                    padding: 8px;
                }
                
                .graph-header {
                    flex-shrink: 0;
                    text-align: center;
                    margin-bottom: 6px;
                }
                
                .graph-container {
                    flex: 1;
                    min-height: 0;
                    position: relative;
                    width: 100%;
                    height: 100%;
                }
                
                .realtime-value {
                    position: absolute;
                    top: 5px;
                    right: 5px;
                    background: rgba(255, 255, 255, 0.9);
                    border: 2px solid #2196F3;
                    border-radius: 6px;
                    padding: 4px 8px;
                    font-weight: bold;
                    font-size: 12px;
                    color: #2196F3;
                    z-index: 100;
                    box-shadow: 0 2px 8px rgba(0,0,0,0.1);
                }
                
                #myChart {
                    width: 100% !important;
                    height: 100% !important;
                    min-height: 150px;
                    border: 1px solid #ccc;
                    border-radius: 4px;
                }
                
                .mobile-buttons {
                    display: none;
                }
            }
        </style>
    )";
}