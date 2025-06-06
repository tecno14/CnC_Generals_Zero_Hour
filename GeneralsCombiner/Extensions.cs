namespace GeneralsCombiner;

public static class Extensions
{
    public static string ReplaceVersionComments(this string content) => content
        .Replace("#\tCommand & Conquer Generals Zero Hour™", "#\tCommand & Conquer Generals™")
        .Replace("**\tCommand & Conquer Generals Zero Hour", "**\tCommand & Conquer Generals");
}
