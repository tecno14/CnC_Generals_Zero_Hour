using System.Text.RegularExpressions;

namespace GeneralsCombiner;

public static class Extensions
{
    public static string ReplaceVersionComments(this string content) => content
        .Replace("#\tCommand & Conquer Generals Zero Hour™", "#\tCommand & Conquer Generals™")
        .Replace("**\tCommand & Conquer Generals Zero Hour", "**\tCommand & Conquer Generals")
        .Replace(";\tCommand & Conquer Generals Zero Hour", ";\tCommand & Conquer Generals");

    public static bool ContainsWord(this string text, string word) =>
        Regex.IsMatch(text, $@"\b{Regex.Escape(word)}\b", RegexOptions.IgnoreCase);
}
