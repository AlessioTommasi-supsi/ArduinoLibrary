#include "cssMultiplexLayout.h"

String cssMultiplexLayout::getCSS() {
    return R"(
        <style>
            /* Desktop Layout - Horizontal split */
            @media (min-width: 1024px) {
                .desktop-multiplex-layout {
                    display: grid;
                    grid-template-columns: 380px 1fr;
                    grid-template-rows: auto 1fr;
                    gap: 15px;
                    height: 100vh;
                    padding: 10px;
                    box-sizing: border-box;
                    overflow: hidden;
                }
                
                .multiplex-header {
                    grid-column: 1 / -1;
                    flex-shrink: 0;
                    padding: 8px;
                    background: #f8f9fa;
                    border-radius: 6px;
                    text-align: center;
                }
                
                .multiplex-controls {
                    grid-column: 1;
                    display: flex;
                    flex-direction: column;
                    gap: 12px;
                    padding: 15px;
                    background: white;
                    border: 1px solid #ddd;
                    border-radius: 8px;
                    overflow-y: auto;
                    max-height: calc(100vh - 120px);
                }
                
                .multiplex-graph {
                    grid-column: 2;
                    display: flex;
                    flex-direction: column;
                    min-height: 0;
                    background: white;
                    border: 1px solid #ddd;
                    border-radius: 8px;
                    padding: 15px;
                }
                
                .graph-header {
                    flex-shrink: 0;
                    text-align: center;
                    margin-bottom: 10px;
                }
                
                .graph-container {
                    flex: 1;
                    min-height: 0;
                    position: relative;
                }
                
                .realtime-value {
                    position: absolute;
                    top: 10px;
                    right: 10px;
                    background: rgba(255, 255, 255, 0.9);
                    border: 2px solid #2196F3;
                    border-radius: 8px;
                    padding: 8px 12px;
                    font-weight: bold;
                    font-size: 16px;
                    color: #2196F3;
                    z-index: 100;
                    box-shadow: 0 2px 8px rgba(0,0,0,0.1);
                }
                
                #myChart {
                    width: 100% !important;
                    height: 100% !important;
                    min-height: 300px;
                }
            }
            
            /* Mobile Layout - Vertical with iOS toggles */
            @media (max-width: 1023px) {
                .desktop-multiplex-layout {
                    display: flex;
                    flex-direction: column;
                    height: 100vh;
                    padding: 10px;
                    box-sizing: border-box;
                    overflow: hidden;
                }
                
                .multiplex-header {
                    flex-shrink: 0;
                    padding: 8px;
                    background: #f8f9fa;
                    border-radius: 6px;
                    text-align: center;
                    margin-bottom: 10px;
                }
                
                .multiplex-controls {
                    flex-shrink: 0;
                    display: flex;
                    flex-direction: column;
                    gap: 10px;
                    padding: 12px;
                    background: white;
                    border: 1px solid #ddd;
                    border-radius: 8px;
                    margin-bottom: 10px;
                    max-height: 250px;
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
                    padding: 12px;
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
                    font-size: 14px;
                    color: #2196F3;
                    z-index: 100;
                    box-shadow: 0 2px 8px rgba(0,0,0,0.1);
                }
                
                #myChart {
                    width: 100% !important;
                    height: 100% !important;
                    min-height: 200px;
                }
                
                .mobile-buttons {
                    display: none;
                }
            }
        </style>
    )";
}