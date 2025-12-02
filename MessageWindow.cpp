#include "MessageWindow.h"

void MessageWindow::Show(const String& msg)
{
    fullText = msg;
    displayedText = "";
    typewriterSource = "";
    visible = true;

    pagesBuilt = false;
    pages.clear();
    currentPage = 0;
    pageComplete = false;

    lastCharTime = millis();
}

void MessageWindow::Hide()
{
    visible = false;
}

void MessageWindow::BuildPages(LGFX_Sprite* ui)
{
    if (pagesBuilt) return;
    pagesBuilt = true;

    const int maxWidth = width - innerPadding * 2;

    // 1. Proper word wrap
    String wrapped = WrapTextToWidth(ui, fullText, maxWidth);

    // 2. Split into lines
    std::vector<String> lines;
    int start = 0;
    while (true)
    {
        int idx = wrapped.indexOf('\n', start);
        if (idx == -1)
        {
            String last = wrapped.substring(start);
            if (last.length()) lines.push_back(last);
            break;
        }
        lines.push_back(wrapped.substring(start, idx));
        start = idx + 1;
    }

    // 3. Pack lines into 3-line pages
    String page = "";
    for (int i = 0; i < (int)lines.size(); ++i)
    {
        page += lines[i];

        if ((i % 3) == 2 || i == (int)lines.size() - 1)
        {
            pages.push_back(page);
            page = "";
        }
        else
        {
            page += "\n";
        }
    }

    // Initialize typing
    if (!pages.empty())
    {
        currentPage = 0;
        typewriterSource = pages[0];
        displayedText = "";
        pageComplete = false;
    }
}

void MessageWindow::Draw(LGFX_Sprite* ui)
{
    if (!visible) return;

    BuildPages(ui);

    // Window position bottom-center
    int x = (ui->width() - width) / 2;
    int y = ui->height() - height - margin;

    ui->fillRoundRect(x, y, width, height, 6, TFT_DARKGREY);
    ui->drawRoundRect(x, y, width, height, 6, TFT_WHITE);

    ui->setTextColor(TFT_WHITE);
    ui->setTextSize(1);

    int lineHeight = ui->fontHeight();   // <-- FIX for scaled text
    int drawX = x + innerPadding;
    int drawY = y + innerPadding;

    // Draw the current visible text (typewriter)
    String& txt = displayedText;
    int start = 0, idx;
    while ((idx = txt.indexOf('\n', start)) != -1)
    {
        ui->setCursor(drawX, drawY);
        ui->print(txt.substring(start, idx));
        drawY += lineHeight;
        start = idx + 1;
    }

    if (start < txt.length())
    {
        ui->setCursor(drawX, drawY);
        ui->print(txt.substring(start));
    }

    // Page arrow
    if (pageComplete && currentPage + 1 < (int)pages.size())
    {
        if ((millis() / 400) % 2 == 0)
        {
            int ax = x + width - innerPadding - 8;
            int ay = y + height - innerPadding - 6;

            for (int r = 0; r < 6; ++r)
            {
                int w = 1 + (r * 8) / 6;
                int sx = ax + (8 - w) / 2;
                ui->fillRect(sx, ay + r, w, 1, TFT_WHITE);
            }
        }
    }
}

void MessageWindow::Update()
{
    if (!visible || !pagesBuilt) return;

    unsigned long now = millis();

    if (pageComplete) return;

    if (displayedText.length() < typewriterSource.length())
    {
        if (now - lastCharTime >= charDelay)
        {
            displayedText += typewriterSource[displayedText.length()];
            lastCharTime = now;
        }
    }

    if (displayedText.length() == typewriterSource.length())
        pageComplete = true;
}

bool MessageWindow::AdvancePage()
{
    if (!pagesBuilt) return true;

    if (displayedText.length() < typewriterSource.length())
    {
        displayedText = typewriterSource;
        pageComplete = true;
        return true;
    }

    if (currentPage + 1 < (int)pages.size())
    {
        currentPage++;
        typewriterSource = pages[currentPage];
        displayedText = "";
        pageComplete = false;
        lastCharTime = millis();
        return true;
    }

    Hide();
    return false;
}

//
// Proper word wrapper (guaranteed no mid-word breaks)
//
String MessageWindow::WrapTextToWidth(LGFX_Sprite* ui, const String& s, int maxWidth)
{
    String out = "";
    String line = "";
    String word = "";

    for (int i = 0; i < s.length(); ++i)
    {
        char c = s[i];

        if (c == ' ' || c == '\n')
        {
            int futureWidth = ui->textWidth(line + word);

            if (futureWidth > maxWidth && line.length() > 0)
            {
                out += line + "\n";
                line = word;
            }
            else
            {
                if (line.length() == 0)
                    line = word;
                else
                    line += word;
            }

            if (c == ' ')
                line += " ";

            if (c == '\n')
            {
                out += line + "\n";
                line = "";
            }

            word = "";
        }
        else
        {
            word += c;
        }
    }

    // last word
    if (word.length())
    {
        if (ui->textWidth(line + word) > maxWidth && line.length() > 0)
            out += line + "\n" + word;
        else
            out += line + word;
    }
    else if (line.length())
        out += line;

    return out;
}
