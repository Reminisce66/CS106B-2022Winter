#include "RosettaStone.h"
#include "GUI/SimpleTest.h"
#include "priorityqueue.h"
using namespace std;

Map<string, double> kGramsIn(const string& str, int kGramLength) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     * (void) str;
     * (void) kGramLength;
     * return {};
     */
    if (kGramLength <= 0) {
        error("kGramLength should be positive.");
    }

    Map<string, double> kgram;
    if (str.size() < kGramLength) return {};
    for (int i=0; i<str.size()-kGramLength+1; ++i) {
        kgram[str.substr(i, kGramLength)]++;
    }

    return kgram;
}

Map<string, double> normalize(const Map<string, double>& input) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     * (void) input;
     * return {};
     */
    if (input.isEmpty()) {
        error("input can not be empty.");
    }
    double square_sum = 0;
    for (const string& str : input) {
        square_sum += pow(input[str], 2);
    }
    if (square_sum == 0) {
        error("input should contain at least one nonzero value.");
    }
    double square_root = sqrt(square_sum);
    Map<string, double> ans;
    for (const string& str : input) {
        ans[str] = input[str]/square_root;
    }

    return ans;
}

Map<string, double> topKGramsIn(const Map<string, double>& source, int numToKeep) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     * (void) source;
     * (void) numToKeep;
     * return {};
     */
    if (numToKeep < 0) {
        error("numToKeep should not be negative.");
    }
    if (numToKeep > source.size()) {
        return source;
    }
    if (numToKeep == 0) {
        return {};
    }
    Map<string, double> ans;
    PriorityQueue<string> pq;
    for (const string& str : source) {
        pq.enqueue(str, source[str]);
    }
    for (int i=0; i<source.size()-numToKeep; ++i) {
        pq.dequeue();
    }
    while (!pq.isEmpty()) {
        const string& str = pq.dequeue();
        ans[str] = source[str];
    }

    return ans;
}

double cosineSimilarityOf(const Map<string, double>& lhs, const Map<string, double>& rhs) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     * (void) lhs;
     * (void) rhs;
     * return {};
     */
    double cosine_ans = 0;
    for (const string& str : lhs) {
        if (rhs.containsKey(str)) {
            cosine_ans += lhs[str] * rhs[str];
        }
    }
    return cosine_ans;
}

string guessLanguageOf(const Map<string, double>& textProfile,
                       const Set<Corpus>& corpora) {
    /* TODO: Delete this comment and the other lines here, then implement
     * this function.
     * (void) textProfile;
     * (void) corpora;
     * return "";
     */
    if (corpora.isEmpty()) {
        error("corpora should not be empty.");
    }

    string ans;
    double max_similarity = 0;
    for (const Corpus& corpus : corpora) {
        double tmp = cosineSimilarityOf(textProfile, corpus.profile);
        if (tmp > max_similarity) {
            max_similarity = tmp;
            ans = corpus.name;
        }
    }

    return ans;
}




/* * * * *   Test Cases Below This Point   * * * * */




PROVIDED_TEST("kGramsIn works when the text length exactly matches the k-gram length.") {
    Map<string, double> expected = {
        { "^_^", 1 }
    };

    EXPECT_EQUAL(kGramsIn("^_^", 3), expected);
}

PROVIDED_TEST("kGramsIn works when the text length is one more than the k-gram length.") {
    Map<string, double> expected = {
        { "the", 1 },
        { "hem", 1 }
    };

    EXPECT_EQUAL(kGramsIn("them", 3), expected);
}

PROVIDED_TEST("kGramsIn works when the text length is one more than the k-gram length.") {
    /* First check: when no k-grams are repeated. */
    Map<string, double> expected = {
        { "the", 1 },
        { "hem", 1 }
    };

    EXPECT_EQUAL(kGramsIn("them", 3), expected);

    /* Next check: when the k-grams are repeated. */
    expected = {
        { "aaa", 2 }
    };

    EXPECT_EQUAL(kGramsIn("aaaa", 3), expected);
}

PROVIDED_TEST("kGramsIn works on a sample when k = 1.") {
    Map<string, double> expected = {
        { "A", 7 },
        { "B", 2 },
        { "D", 1 },
        { "N", 4 },
    };

    EXPECT_EQUAL(kGramsIn("ABANANABANDANA", 1), expected);
}

PROVIDED_TEST("kGramsIn works on a sample when k = 2.") {
    Map<string, double> expected = {
        { "AB", 2 },
        { "AN", 4 },
        { "BA", 2 },
        { "DA", 1 },
        { "NA", 3 },
        { "ND", 1 }
    };

    EXPECT_EQUAL(kGramsIn("ABANANABANDANA", 2), expected);
}

PROVIDED_TEST("kGramsIn handles non-English strings.") {
    /* The characters in these strings are expressed in UTF-8. This
     * means that splitting the string into k-grams will produce some
     * k-grams containing fragments of individual characters. See
     * the assignment description for more details about what's
     * going on here.
     */
    string devanagari = "दे";
    Map<string, double> expected = {
      {"\244\246\340", 1},
      {"\246\340\245", 1},
      {"\340\244\246", 1},
      {"\340\245\207", 1}
    };
    EXPECT_EQUAL(kGramsIn(devanagari, 3), expected);

    string ktavAshuri = "דָג";
    expected = {
        {"\223\326\270", 1},
        {"\270\327\222", 1},
        {"\326\270\327", 1},
        {"\327\223\326", 1}
    };
    EXPECT_EQUAL(kGramsIn(ktavAshuri, 3), expected);
}

PROVIDED_TEST("kGramsIn works when the input is shorter than the k-gram length.") {
    EXPECT_EQUAL(kGramsIn("ABANANABANDANA", 137), {});
}

PROVIDED_TEST("kGramsIn reports errors on bad inputs.") {
    EXPECT_ERROR(kGramsIn("Oops, negative", -137));
    EXPECT_ERROR(kGramsIn("Oops, zero",     0));
}

PROVIDED_TEST("normalize does not add or remove keys.") {
    Map<string, double> incomplete = {
        { "C", 1 },
        { "O", 1 },
        { "N", 1 },
        { "E", 1 }
    };

    auto result = normalize(incomplete);
    EXPECT(result.containsKey("C"));
    EXPECT(result.containsKey("O"));
    EXPECT(result.containsKey("N"));
    EXPECT(result.containsKey("E"));
    EXPECT_EQUAL(result.size(), 4);
}

PROVIDED_TEST("normalize works on positive numbers.") {
    Map<string, double> scores = {
        { "C", 1 },
        { "O", 1 },
        { "N", 1 },
        { "E", 1 },
    };
    Map<string, double> expected = {
        { "C", 0.5 },
        { "O", 0.5 },
        { "N", 0.5 },
        { "E", 0.5 },
    };

    EXPECT_EQUAL(normalize(scores), expected);
}

PROVIDED_TEST("normalize works on negative numbers.") {
    Map<string, double> scores = {
        { "C", -1 },
        { "O", -1 },
        { "N", -1 },
        { "E", -1 },
    };
    Map<string, double> expected = {
        { "C", -0.5 },
        { "O", -0.5 },
        { "N", -0.5 },
        { "E", -0.5 },
    };

    EXPECT_EQUAL(normalize(scores), expected);
}

PROVIDED_TEST("normalize works on unequal values.") {
    Map<string, double> scores = {
        { "O",  3 },
        { "N", -4 },
    };
    Map<string, double> expected = {
        { "O",  0.6 },
        { "N", -0.8 },
    };

    EXPECT_EQUAL(normalize(scores), expected);
}

PROVIDED_TEST("normalize works on huge numbers.") {
    /* As mentioned in the assignment description, the int type
     * is (usually) limited to a range of roughly negative two
     * billion to roughly positive two billion. The double type,
     * however, can handle this just fine. This test case checks
     * to make sure that large numbers are processed correctly.
     * If you store any intermediary calculation values in an
     * int rather than a double, it may cause this test
     * to fail.
     */
    Map<string, double> scores = {
        { "singleton", 1e18 }
    };

    auto result = normalize(scores);
    EXPECT_EQUAL(result.size(), 1);
    EXPECT(result.containsKey("singleton"));

    /* doubles store rounded values, so with large values being
     * calculated we may end up with a value that's slightly
     * wrong. These error bounds should catch this.
     */
    EXPECT_LESS_THAN_OR_EQUAL_TO(result["singleton"], 10);
    EXPECT_GREATER_THAN_OR_EQUAL_TO(result["singleton"], 0.0);
}

PROVIDED_TEST("normalize reports errors on inputs that are all zero.") {
    EXPECT_ERROR(normalize({}));
    EXPECT_ERROR(normalize({{"A", 0}, {"C", 0}, {"E", 0}}));
}

PROVIDED_TEST("topKGramsIn finds the most frequent k-gram.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    Map<string, double> expected = {
        { "B", 30 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 1), expected);
}

PROVIDED_TEST("topKGramsIn finds the top two k-grams.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    Map<string, double> expected = {
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 2), expected);
}

PROVIDED_TEST("topKGramsIn works if all k-grams are requested.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 3), input);
}

PROVIDED_TEST("topKGramsIn works if more k-grams are requested than exist.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 137), input);
}

PROVIDED_TEST("topKGramsIn works if no k-grams are requested.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 0), {});
}

PROVIDED_TEST("topKGramsIn reports errors on bad inputs.") {
    Map<string, double> input = {
        { "A", 10 },
        { "B", 30 },
        { "C", 20 }
    };

    EXPECT_ERROR(topKGramsIn(input, -137));
}

PROVIDED_TEST("topKGramsIn works even if frequencies are negative.") {
    Map<string, double> input = {
        { "A",  2 },
        { "B",  1 },
        { "C",  0 },
        { "D", -1 },
        { "E", -2 },
        { "F", -3 },
        { "G", -4 },
        { "H", -5 }
    };

    Map<string, double> expected = {
        { "A",  2 },
        { "B",  1 },
        { "C",  0 },
        { "D", -1 },
        { "E", -2 },
    };

    EXPECT_EQUAL(topKGramsIn(input, 5), expected);
}

PROVIDED_TEST("topKGramsIn works with fractional weights.") {
    Map<string, double> input = {
        { "A", 10.1 },
        { "B", 10.2 },
        { "C", 10.3 }
    };
    Map<string, double> expected = {
        { "C", 10.3 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 1), expected);

    input = {
        { "A", 10.3 },
        { "B", 10.2 },
        { "C", 10.1 }
    };
    expected = {
        { "A", 10.3 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 1), expected);

    input = {
        { "A", 10.2 },
        { "B", 10.3 },
        { "C", 10.1 }
    };
    expected = {
        { "B", 10.3 }
    };

    EXPECT_EQUAL(topKGramsIn(input, 1), expected);
}

PROVIDED_TEST("cosineSimilarityOf works with one shared key.") {
    Map<string, double> one = {
        { "A",  1 },
    };
    Map<string, double> two = {
        { "A",  1 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), 1);
}

PROVIDED_TEST("cosineSimilarityOf works with two maps with non-overlapping keys.") {
    Map<string, double> one = {
        { "A",  1 },
    };
    Map<string, double> two = {
        { "B",  1 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), 0);
}

PROVIDED_TEST("cosineSimilarityOf works where keys are present in RHS but not LHS.") {
    /* Yes, we know these aren't normalized and that we promised that we would normalize
     * all inputs to cosineSimilarityOf. This is just for testing purposes.
     */
    Map<string, double> one = {
        { "O",  1 },
        { "C",  2 },
        { "E",  3 },
        { "A",  4 },
        { "N",  5 },
    };
    Map<string, double> two = {
        { "C", -2 },
        { "E", -3 },
        { "A", -4 },
        { "N", -5 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), -54);
}

PROVIDED_TEST("cosineSimilarityOf works where keys are present in LHS but not RHS.") {
    /* Yes, we know these aren't normalized and that we promised that we would normalize
     * all inputs to cosineSimilarityOf. This is just for testing purposes.
     */
    Map<string, double> one = {
        { "C",  2 },
        { "E",  3 },
        { "A",  4 },
        { "N",  5 },
    };
    Map<string, double> two = {
        { "O",  1 },
        { "C", -2 },
        { "E", -3 },
        { "A", -4 },
        { "N", -5 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), -54);
}

PROVIDED_TEST("cosineSimilarityOf works with keys missing from both sides.") {
    /* Yes, we know these aren't normalized and that we promised that we would normalize
     * all inputs to cosineSimilarityOf. This is just for testing purposes.
     */
    Map<string, double> one = {
        { "C",  2 },
        { "E",  3 },
        { "A",  4 },
    };
    Map<string, double> two = {
        { "O",  1 },
        { "E", -3 },
        { "A", -4 },
    };
    EXPECT_EQUAL(cosineSimilarityOf(one, two), -25);
}

PROVIDED_TEST("guessLanguageOf works with perfect similarity.") {
    Corpus o = { "Language O", {{ "O", 1 }} },
           c = { "Language C", {{ "C", 1 }} },
           e = { "Language E", {{ "E", 1 }} },
           a = { "Language A", {{ "A", 1 }} },
           n = { "Language N", {{ "N", 1 }} };

    EXPECT_EQUAL(guessLanguageOf({{ "O", 1 }}, {o, c, e, a, n}), o.name);
    EXPECT_EQUAL(guessLanguageOf({{ "C", 1 }}, {o, c, e, a, n}), c.name);
    EXPECT_EQUAL(guessLanguageOf({{ "E", 1 }}, {o, c, e, a, n}), e.name);
    EXPECT_EQUAL(guessLanguageOf({{ "A", 1 }}, {o, c, e, a, n}), a.name);
    EXPECT_EQUAL(guessLanguageOf({{ "N", 1 }}, {o, c, e, a, n}), n.name);
}

PROVIDED_TEST("guessLanguageOf works with imperfect similarity.") {
    Corpus o = { "Language O", {{ "O", 2 }, {"C", 1}} },
           c = { "Language C", {{ "C", 2 }, {"E", 1}} },
           e = { "Language E", {{ "E", 2 }, {"A", 1}} },
           a = { "Language A", {{ "A", 2 }, {"N", 1}} },
           n = { "Language N", {{ "N", 2 }, {"O", 1}} };

    EXPECT_EQUAL(guessLanguageOf({{"O", 1}}, {o, c, e, a, n}), o.name);
    EXPECT_EQUAL(guessLanguageOf({{"C", 1}}, {o, c, e, a, n}), c.name);
    EXPECT_EQUAL(guessLanguageOf({{"E", 1}}, {o, c, e, a, n}), e.name);
    EXPECT_EQUAL(guessLanguageOf({{"A", 1}}, {o, c, e, a, n}), a.name);
    EXPECT_EQUAL(guessLanguageOf({{"N", 1}}, {o, c, e, a, n}), n.name);
}

PROVIDED_TEST("guessLanguageOf works on more complex inputs.") {
    Corpus karel  = { "Karel",  {{"R", 1}, {"O", 2}, {"B", 1}, {"T", 1}} };
    Corpus trisha = { "Trisha", {{"T", 1}, {"R", 1}, {"I", 1}, {"S", 1}, {"H", 1}, {"A", 1}} };
    Corpus chris  = { "Chris",  {{"C", 1}, {"H", 1}, {"R", 1}, {"I", 1}, {"S", 1}} };
    Corpus jerry  = { "Jerry",  {{"J", 1}, {"E", 1}, {"R", 2}, {"Y", 1}} };

    EXPECT_EQUAL(guessLanguageOf({{"R", 1}}, {karel, trisha, chris, jerry}), jerry.name);
}

PROVIDED_TEST("guessLanguageOf reports errors if no corpora are provided.") {
    EXPECT_ERROR(guessLanguageOf({}, {}));
    EXPECT_ERROR(guessLanguageOf({{"C", -1}}, {}));
}
